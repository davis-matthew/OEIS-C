/*
 * A160778  Numbers that have an "i" in the middle of their names in Spanish.
 * https://oeis.org/A160778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = the n‑th positive integer whose Spanish name has an 'i'
 *            in the exact middle character (no spaces or accents)
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Append a C string to a buffer. */
static void append(const char *s, char *buf, size_t *pos)
{
    while (*s) buf[(*pos)++] = *s++;
}

/* Build the Spanish name of n (1 ≤ n). The name is written without
   spaces or accents, the result is null‑terminated in *buf. */
static void build_name(uint64_t n, char *buf, size_t *pos)
{
    if (n >= 1000000ULL) {
        uint64_t millions = n / 1000000ULL;
        uint64_t rest = n % 1000000ULL;
        if (millions == 1) {
            append("un", buf, pos);
            append("million", buf, pos);   /* un million */
        } else {
            build_name(millions, buf, pos);
            append("millones", buf, pos);
        }
        if (rest) build_name(rest, buf, pos);
        return;
    }
    if (n >= 1000ULL) {
        uint64_t thousands = n / 1000ULL;
        uint64_t rest = n % 1000ULL;
        if (thousands == 1) {
            append("mil", buf, pos);
        } else {
            build_name(thousands, buf, pos);
            append("mil", buf, pos);
        }
        if (rest) build_name(rest, buf, pos);
        return;
    }
    if (n >= 200ULL) {
        static const char *const hundreds[10] = {
            "", "", "doscientos", "trescientos", "cuatrocientos",
            "quinientos", "seiscientos", "setecientos",
            "ochocientos", "novecientos"
        };
        uint64_t h = n / 100ULL;
        uint64_t rem = n % 100ULL;
        append(hundreds[h], buf, pos);
        if (rem) build_name(rem, buf, pos);
        return;
    }
    if (n >= 100ULL) {
        if (n == 100ULL) {
            append("cien", buf, pos);
        } else {
            append("ciento", buf, pos);
            build_name(n - 100ULL, buf, pos);
        }
        return;
    }
    if (n >= 20ULL) {
        if (n < 30ULL) {
            if (n == 20ULL) {
                append("veinte", buf, pos);
            } else {
                append("veinti", buf, pos);
                build_name(n - 20ULL, buf, pos);
            }
            return;
        }
        static const char *const tens[10] = {
            "", "", "veinte", "treinta", "cuarenta",
            "cincuenta", "sesenta", "setenta",
            "ochenta", "noventa"
        };
        uint64_t t = n / 10ULL;
        uint64_t rem = n % 10ULL;
        append(tens[t], buf, pos);
        if (rem) {
            append("y", buf, pos);
            build_name(rem, buf, pos);
        }
        return;
    }
    static const char *const units[10] = {
        "", "uno", "dos", "tres", "cuatro",
        "cinco", "seis", "siete", "ocho", "nueve"
    };
    if (n < 10ULL) {
        append(units[n], buf, pos);
        return;
    }
    static const char *const teens[10] = {
        "diez", "once", "doce", "trece", "catorce",
        "quince", "dieciseis", "diecisiete",
        "dieciocho", "diecinueve"
    };
    append(teens[n - 10], buf, pos);
}

/* Test whether num has an 'i' in the exact middle of its Spanish name. */
static int middle_i_candidate(uint64_t num)
{
    char str[2048];
    size_t pos = 0;
    build_name(num, str, &pos);
    if (pos % 2 == 0) return 0;
    return str[pos / 2] == 'i';
}

/* OEIS sequence A160778 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */
    uint64_t count = 0;
    uint64_t candidate = 15;            /* first term is 16 */
    while (count < n) {
        ++candidate;
        if (middle_i_candidate(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
    }
    return 0; /* unreachable */
}
