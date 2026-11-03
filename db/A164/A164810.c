/*
 * A164810  a(n) = smallest number which has in its Spanish name the letter
 *          "d" in the n-th position,or -1 if no such number exists.
 * https://oeis.org/A164810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, sign
 * Formula:   a(n) = smallest integer whose Spanish name (without spaces or
 *            hyphens) contains the letter 'd' in position n (positions are
 *            1‑based), or -1 if no such number exists
 * Range:     i8 n <= 3, i16 n <= 33, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Spanish number writer and memoised solver for A164810 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: append a null‑terminated string to buffer at current position. */
static inline void append_buf(const char *s, char *buf, size_t *pos)
{
    size_t l = strlen(s);
    memcpy(buf + *pos, s, l);
    *pos += l;
}

/* Append word for hundreds 200‑999. */
static inline void append_hundreds(unsigned d, char *buf, size_t *pos)
{
    static const char *HUND[10] = {
        "", "", "doscientos", "trescientos",
        "cuatrocientos", "quinientos", "seiscientos",
        "setecientos", "ochocientos", "novecientos"
    };
    if (d < 10)
        append_buf(HUND[d], buf, pos);
}

/* Append word for 10‑19. */
static inline void append_teens(unsigned d, char *buf, size_t *pos)
{
    static const char *TEENS[10] = {
        "diez", "once", "doce", "trece", "catorce",
        "quince", "dieciseis", "diecisiete",
        "dieciocho", "diecinueve"
    };
    if (d < 10)
        append_buf(TEENS[d], buf, pos);
}

/* Append word for tens 30‑99. */
static inline void append_tens(unsigned d, char *buf, size_t *pos)
{
    static const char *TENS[10] = {
        "", "", "", "treinta", "cuarenta", "cincuenta",
        "sesenta", "setenta", "ochenta", "noventa"
    };
    if (d < 10)
        append_buf(TENS[d], buf, pos);
}

/* Append word for 1‑9. */
static inline void append_units(unsigned d, char *buf, size_t *pos)
{
    static const char *ONES[10] = {
        "", "uno", "dos", "tres", "cuatro", "cinco",
        "seis", "siete", "ocho", "nueve"
    };
    if (d < 10)
        append_buf(ONES[d], buf, pos);
}

/* Write numbers 1‑999 into buf. */
static void write_below_1000(idx_t n, char *buf, size_t *pos)
{
    if (n == 0)
        return;

    if (n >= 100) {
        unsigned h = (unsigned)(n / 100);
        n %= 100;
        if (h == 1) { /* 100‑199 */
            if (n == 0) {
                append_buf("cien", buf, pos);
                return;
            }
            append_buf("ciento", buf, pos);
        } else { /* 200‑999 */
            append_hundreds(h, buf, pos);
            if (n == 0)
                return;
        }
    }

    if (n >= 20) {
        if (n < 30) {
            static const char *VINT2[10] = {
                "veinte", "veintiuno", "veintidos", "veintitr",
                "veinticuatro", "veinticinco", "veintiseis",
                "veintisiete", "veintiocho", "veintinueve"
            };
            append_buf(VINT2[n - 20], buf, pos);
            return;
        }
        unsigned t = (unsigned)(n / 10);
        unsigned u = (unsigned)(n % 10);
        append_tens(t, buf, pos);
        if (u != 0) {
            append_buf("y", buf, pos);
            append_units(u, buf, pos);
        }
        return;
    }

    if (n >= 10) { /* 10‑19 */
        append_teens((unsigned)(n - 10), buf, pos);
        return;
    }

    /* 1‑9 */
    append_units((unsigned)n, buf, pos);
}

/* Main routine: write integer n (>=0) in Spanish, no spaces/hyphens. */
static void write_number(idx_t n, char *buf, size_t *len_out)
{
    size_t pos = 0;
    if (n == 0) {
        append_buf("cero", buf, &pos);
        *len_out = pos;
        return;
    }

    /* Millions */
    if (n >= 1000000) {
        idx_t millions = n / 1000000;
        n %= 1000000;
        if (millions == 1) {
            append_buf("un", buf, &pos);
            append_buf("millon", buf, &pos);
        } else {
            write_below_1000(millions, buf, &pos);
            append_buf("millones", buf, &pos);
        }
    }

    /* Thousands */
    if (n >= 1000) {
        idx_t thousands = n / 1000;
        n %= 1000;
        if (thousands == 1) {
            append_buf("mil", buf, &pos);
        } else {
            write_below_1000(thousands, buf, &pos);
            append_buf("mil", buf, &pos);
        }
    }

    /* Remaining <1000 */
    if (n > 0)
        write_below_1000(n, buf, &pos);

    *len_out = pos;
}

/* ---------- Memoised solver ---------- */

/* answer_tbl[pos-1] = smallest number having a 'd' at position pos;
   0 means unknown (since no zero number in this context). */
static seq_t *answer_tbl = NULL;
static size_t answer_cap = 0;
static idx_t next_to_process = 1;

/* Resize table if needed. */
static void expand_answers(size_t needed)
{
    if (needed <= answer_cap)
        return;
    size_t new_cap = answer_cap ? answer_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, answer_tbl, new_cap);
    if (!tmp)
        return; /* allocation failure – rare */
    mem_set(tmp + answer_cap, 0, (new_cap - answer_cap) * sizeof(seq_t));
    answer_tbl = tmp;
    answer_cap = new_cap;
}

/* Generate numbers until the target position is known or a reasonable limit is reached. */
static void ensure_answer(idx_t target)
{
    const idx_t SEARCH_LIMIT = 5000000ULL; /* sufficient for first 10 000 positions */
    if (target == 0)
        return;

    while (true) {
        if (target <= answer_cap && answer_tbl[target - 1] != 0)
            return;                /* already known */

        if (next_to_process > SEARCH_LIMIT)
            return;                /* give up – treat as missing */

        /* Build Spanish representation of next_to_process. */
        char buf[512];
        size_t len;
        write_number(next_to_process, buf, &len);

        /* Scan for 'd' in the name. */
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == 'd') {
                size_t pos = i + 1;               /* 1‑based position */
                expand_answers(pos);
                if (answer_tbl[pos - 1] == 0) {
                    answer_tbl[pos - 1] = (seq_t)next_to_process;
                }
            }
        }
        ++next_to_process;
    }
}

/* OEIS API function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1; /* outside offset */

    /* Initialise memoisation table on first call. */
    if (answer_tbl == NULL) {
        answer_cap = 64;
        answer_tbl = mem_new(seq_t, answer_cap); /* 0 = unknown */
    }

    ensure_answer(n);
    if (n <= answer_cap && answer_tbl[n - 1] != 0)
        return answer_tbl[n - 1];
    return (seq_t)-1; /* no such number found within search limit */
}
