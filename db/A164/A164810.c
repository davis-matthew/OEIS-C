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
 * Helpers:   append_buf, append_hundreds, append_teens, append_tens,
 *            append_units, write_below_1000, write_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Spanish number writer and memoised solver for A164810 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/append_buf.h"
#include "../utils/append_hundreds.h"
#include "../utils/append_teens.h"
#include "../utils/append_tens.h"
#include "../utils/append_units.h"
#include "../utils/write_below_1000.h"
#include "../utils/write_number.h"

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
    seq_t *tmp = (seq_t *)realloc(answer_tbl, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – rare */
    memset(tmp + answer_cap, 0, (new_cap - answer_cap) * sizeof(seq_t));
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
        answer_tbl = (seq_t *)calloc(answer_cap, sizeof(seq_t)); /* 0 = unknown */
    }

    ensure_answer(n);
    if (n <= answer_cap && answer_tbl[n - 1] != 0)
        return answer_tbl[n - 1];
    return (seq_t)-1; /* no such number found within search limit */
}
