/*
 * A063311  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          38 ).
 * https://oeis.org/A063311
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-4) + a(n-6) - a(n-10)  for n > 13
 * Initial values (offset 2):
 *   a(2) = -1, a(3) = 14, a(4) = 30, a(5) = 45, a(6) = 60, a(7) = 75,
 *   a(8) = 90, a(9) = 103, a(10) = 120, a(11) = 133, a(12) = 150, a(13) = 165
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* dynamic cache of already computed terms */
static seq_t *a_tbl = NULL;        /* array of computed terms */
static idx_t tbl_size   = 0;       /* current allocated size (number of elements) */
static idx_t computed_upto = 0;    /* largest index for which a_tbl[] is valid */

/* helper to ensure the cache is large enough for up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n + 1 > tbl_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_tbl = (seq_t *)realloc(a_tbl, new_size * sizeof(seq_t));
        if (!new_tbl) {
            /* allocation failed – silently fall back to undefined behaviour */
            return;
        }
        a_tbl = new_tbl;
        tbl_size = new_size;
    }
}

/* initialize the first known terms a(2)..a(13) */
static void init_known_terms(void)
{
    /* ensure enough space for indices up to 13 */
    ensure_capacity(13);

    /* fill with zeros for indices < 2 (not used, but defined for safety) */
    a_tbl[0] = (seq_t)0;
    a_tbl[1] = (seq_t)0;

    a_tbl[2]  = (seq_t)-1;
    a_tbl[3]  = (seq_t)14;
    a_tbl[4]  = (seq_t)30;
    a_tbl[5]  = (seq_t)45;
    a_tbl[6]  = (seq_t)60;
    a_tbl[7]  = (seq_t)75;
    a_tbl[8]  = (seq_t)90;
    a_tbl[9]  = (seq_t)103;
    a_tbl[10] = (seq_t)120;
    a_tbl[11] = (seq_t)133;
    a_tbl[12] = (seq_t)150;
    a_tbl[13] = (seq_t)165;

    computed_upto = 13;
}

seq_t a(idx_t n)
{
    /* behaviour for indices below offset is arbitrary – return 0 */
    if (n < 2) return (seq_t)0;

    /* first use of the function – initialise known terms */
    if (!a_tbl) init_known_terms();

    /* extend the cache if needed */
    if (n > computed_upto) {
        /* grow cache to cover up to n */
        ensure_capacity(n);

        /* compute sequentially from (computed_upto + 1) up to n */
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            if (i <= 13) {
                /* the table already contains the initial value – nothing to do */
                /* (the values are already set during init_known_terms) */
            } else {
                /* recurrence: a(n) = a(n-4) + a(n-6) - a(n-10) */
                a_tbl[i] = a_tbl[i-4] + a_tbl[i-6] - a_tbl[i-10];
            }
        }
        computed_upto = n;
    }

    return a_tbl[n];
}
