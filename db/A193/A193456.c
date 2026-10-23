/*
 * A193456  Paradigm shift sequence with procedure length p=4.
 * https://oeis.org/A193456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A193456 – Paradigm Shift Sequence with procedure length p=4.
 *
 * a(n) = m^(C−R)⋅(m+1)^R
 *
 * Computation steps:
 *   1. Partition indices into “blocks” of lengths:
 *          C=1 → 10 terms      (n=1…10)
 *          C=2 → 10 terms      (n=11…20)
 *          C≥3 : 9,10,9,10,…  (odd C → 9 terms, even C → 10 terms)
 *      The block number C for a given n is derived from this pattern.
 *   2. Let S = n + 4.
 *   3. Let m = ⌊S / C⌋ − 4.
 *   4. Let R = S mod C.
 *   5. Return m^(C−R) * (m+1)^R.
 *
 * This implementation works for any unsigned integer type seq_t
 * (including arbitrary‑precision integer classes).
 */

#include <stddef.h>  /* for size_t, if needed */

static idx_t compute_C(idx_t n)
{
    if (n <= 10) return 1;
    if (n <= 20) return 2;

    /* n > 20 */
    idx_t delta = n - 20;                 /* 1 for n=21 */
    idx_t pair_index = (delta - 1) / 19;   /* number of finished 19‑term pairs */
    idx_t r = (delta - 1) % 19;           /* position inside the current pair */
    idx_t base_C = 3 + 2 * pair_index;    /* odd C of this pair */

    if (r < 9)   /* inside the odd “9‑term” block */
        return base_C;
    else         /* inside the even “10‑term” block */
        return base_C + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* outside defined range – arbitrary choice */

    idx_t C = compute_C(n);
    seq_t S = (seq_t)n + (seq_t)4;
    seq_t m = (seq_t)((S / C) - 4);
    idx_t R = (idx_t)(S % C);

    seq_t result = 1;
    for (idx_t i = 0; i < C - R; ++i) result *= m;
    for (idx_t i = 0; i < R; ++i)     result *= (m + 1);

    return result;
}
