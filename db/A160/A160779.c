/*
 * A160779  Numbers that have an "n" in the middle of their names in Spanish.
 * https://oeis.org/A160779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose Spanish name (without spaces) has an odd
 * length and the middle character is 'n'.
 * Implementation scans natural numbers until the desired count is reached.
 * Spanish conversion supports values up to at least 999 999 999 correctly.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static void append(const char *s, char *buf, size_t *pos)
{
    while (*s) buf[(*pos)++] = *s++;
}

/* Units, teens, tens, hundreds for numbers <1000 */
static const char *const UNIT[]   = {"", "uno", "dos", "tres", "cuatro", "cinco", "seis", "siete", "ocho", "nueve"};
static const char *const TEEN[]   = {"diez", "once", "doce", "trece", "catorce", "quince", "dieciséis", "diecisiete", "dieciocho", "diecinueve"};
static const char *const TWENTY[] = {"veinte", "veintiuno", "veintidós", "veintitrés", "veinticuatro", "veinticinco", "veintiséis", "veintisiete", "veintiocho", "veintinueve"};
static const char *const TENS[]   = {"veinte", "treinta", "cuarenta", "cincuenta", "sesenta", "setenta", "ochenta", "noventa"};
static const char *const HUND[]   = {"", "cien", "doscientos", "trescientos", "cuatrocientos", "quinientos", "seiscientos", "setecientos", "ochocientos", "novecientos"};

/* Helper to convert numbers <1000 */
static void small_to_spanish(uint64_t n, char *buf, size_t *pos)
{
    if (n == 0) return;

    uint64_t h = n / 100;
    uint64_t r = n % 100;
    if (h > 0) {
        if (h == 1 && r == 0)                 /* exactly 100 */
            append(HUND[1], buf, pos);         /* "cien" */
        else if (h == 1)                      /* 101-199 */
            append("ciento", buf, pos);        /* "ciento" */
        else                                  /* 200-900 */
            append(HUND[h], buf, pos);
    }

    if (r == 0) return;

    if (r < 10) {                              /* units */
        append(UNIT[r], buf, pos);
    } else if (r < 20) {                       /* teens */
        append(TEEN[r - 10], buf, pos);
    } else if (r < 30) {                       /* 20-29 special */
        append(TWENTY[r - 20], buf, pos);
    } else {                                   /* 30-99 */
        uint64_t t = r / 10;
        append(TENS[t - 2], buf, pos);          /* indices 0..7 for 20..90 */
        uint64_t u = r % 10;
        if (u > 0) {
            append("y", buf, pos);
            append(UNIT[u], buf, pos);
        }
    }
}

/* Forward declarations for recursion */
static void write_millions(uint64_t n, char *buf, size_t *pos);
static void write_thousands(uint64_t n, char *buf, size_t *pos);

/* Numbers >= 1 000 000 */
static void write_millions(uint64_t n, char *buf, size_t *pos)
{
    uint64_t h = n / 1000000;
    uint64_t rest = n % 1000000;

    if (h == 1) {
        append("un", buf, pos);
        append("million", buf, pos);          /* "unmillion" */
    } else {
        write_thousands(h, buf, pos);
        append("millones", buf, pos);
    }

    if (rest > 0) {
        write_thousands(rest, buf, pos);
    }
}

/* Numbers >= 1 000 < 1 000 000 */
static void write_thousands(uint64_t n, char *buf, size_t *pos)
{
    uint64_t h = n / 1000;
    uint64_t rest = n % 1000;

    if (h == 1) {
        append("mil", buf, pos);              /* 1 000 */
    } else {
        if (h < 1000)
            write_millions(h, buf, pos);      /* recursively handle up to millions */
        else
            write_thousands(h, buf, pos);
        append("mil", buf, pos);
    }

    if (rest > 0) {
        small_to_spanish(rest, buf, pos);
    }
}

/* Convert any positive n to Spanish without spaces */
static void write_spanish(uint64_t n, char *buf, size_t *pos)
{
    if (n >= 1000000) {
        write_millions(n, buf, pos);
    } else if (n >= 1000) {
        write_thousands(n, buf, pos);
    } else {
        small_to_spanish(n, buf, pos);
    }
}

/* Check whether a number has an 'n' in the middle of its name */
static bool has_middle_n(uint64_t x)
{
    char buf[512];
    size_t pos = 0;

    write_spanish(x, buf, &pos);

    if (pos == 0) return false;
    if ((pos & 1) == 0) /* even length -> no single middle char */
        return false;

    size_t mid = (pos - 1) / 2;
    return buf[mid] == 'n';
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t i = 1; ; ++i) {
        if (has_middle_n(i)) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}
