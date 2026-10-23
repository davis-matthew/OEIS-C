/*
 * A180155  a(1)=1; thereafter, a(n) is the smallest integer k not yet in the
 *          sequence, such that k^3 contains in consecutive digits the decimal
 *          representation of a(n-1).
 * https://oeis.org/A180155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 1196, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, seq_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180155: a(1)=1; thereafter a(n) is the smallest integer k not yet occurring
 * in the sequence such that the decimal representation of k³ contains, as
 * consecutive digits, the representation of a(n‑1).  The sequence is built
 * incrementally and memoised so that calls in any order are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
/* utility headers supplied by the build system */
#include "../utils/digit_len.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_to_str.h"

/*-------------------- static state --------------------*/
static seq_t *seq      = NULL; /* 0‑based values of the sequence */
static size_t seq_len  = 0;    /* number of values computed so far */
static size_t seq_cap  = 0;    /* current capacity of `seq` */

static bool *used      = NULL; /* bool array marking numbers already used */
static size_t used_cap = 0;    /* capacity of `used` */

static char **cube_str = NULL; /* cached null‑terminated strings of k³ */
static size_t cube_cap = 0;    /* capacity of `cube_str` */

/* enlarge `seq` so that it holds at least `min_cap` elements */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* enlarge `used` so that index `idx` (inclusive) is valid */
static void ensure_used_cap(size_t idx)
{
    if (used_cap > idx) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    used = (bool *)realloc(used, new_cap * sizeof(bool));
    for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
    used_cap = new_cap;
}

/* enlarge `cube_str` so that index `idx` (inclusive) is valid */
static void ensure_cube_cap(size_t idx)
{
    if (cube_cap > idx) return;
    size_t new_cap = cube_cap ? cube_cap * 2 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    cube_str = (char **)realloc(cube_str, new_cap * sizeof(char *));
    for (size_t i = cube_cap; i < new_cap; ++i) cube_str[i] = NULL;
    cube_cap = new_cap;
}

/* compute (or fetch from cache) the null terminated string of k³ */
static void prepare_cube_str(idx_t k)
{
    size_t kk = (size_t)k;
    ensure_cube_cap(kk);
    if (cube_str[kk] != NULL) return;          /* already computed */

    seq_t cube = seq_pow((seq_t)k, (idx_t)3);
    size_t dlen = digit_len(cube);
    char *buf = (char *)malloc(dlen + 1);
    if (!buf) abort();                         /* memory error */
    seq_to_str(cube, buf, &dlen);
    buf[dlen] = '\0';
    cube_str[kk] = buf;
}

/* return a null terminated string representation of the value `v`
 * (reuses the same buffer across successive calls to avoid re‑allocation). */
static char *get_prev_str(seq_t v)
{
    static char *prev_buf = NULL;
    static size_t prev_cap = 0;
    size_t len = digit_len(v);
    if (prev_cap < len + 1) {
        free(prev_buf);
        prev_buf = (char *)malloc(len + 1);
        if (!prev_buf) abort();
        prev_cap = len + 1;
    }
    seq_to_str(v, prev_buf, &len);
    prev_buf[len] = '\0';
    return prev_buf;
}

/*-----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* out of range, defensive */

    /* initialise first term if necessary */
    if (seq_len == 0) {
        ensure_seq_cap(2);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_used_cap(1);
        used[1] = true;
    }

    /* generate terms until we have at least n items */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        char *prev_str = get_prev_str(prev);

        /* find the smallest unused k whose cube contains prev_str */
        for (idx_t k = 1; ; ++k) {
            ensure_used_cap((size_t)k);
            if (used[(size_t)k]) continue;          /* already in sequence */

            prepare_cube_str(k);
            if (strstr(cube_str[(size_t)k], prev_str) != NULL) {
                /* accept this k as a(n) */
                used[(size_t)k] = true;
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)k;
                break;
            }
        }
    }

    return seq[n - 1];
}
