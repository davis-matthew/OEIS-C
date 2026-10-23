/*
 * A014889  n is equal to the number of 5s in all numbers <= n written in base
 *          7.
 * https://oeis.org/A014889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 761664 + floor((n-1)/7)*823543 + s[(n-1)%7] where s = {0,
 *            55835, 55836, 55843, 55844, 57945, 61879} (prefix sums of the
 *            7‑term increment cycle). This follows from the recurrence a(n) =
 *            a(n-1) + a(n-7) - a(n-8) for n > 7 and the initial values
 *            a(1)=761664, a(2)=817499, ..., a(7)=823543
 * Range:     u8 none, u16 none, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t (uint64_t) and seq_t */

static const seq_t INC_PREFIX[7] = {
    (seq_t)0,        /* remainder 0   : no additional increment   */
    (seq_t)55835,    /* r = 1   -> add 55835                      */
    (seq_t)55836,    /* r = 2   -> add 55835 + 1                  */
    (seq_t)55843,    /* r = 3   -> add 55835 + 1 + 7              */
    (seq_t)55844,    /* r = 4   -> +1 after previous             */
    (seq_t)57945,    /* r = 5   -> +2101 after previous           */
    (seq_t)61879     /* r = 6   -> +3934 after previous           */
};

seq_t a(idx_t n) {
    /* Offset is 1, so n >= 1.  All arithmetic is performed in seq_t. */
    idx_t cycles = (n - 1) / 7;
    idx_t rem    = (n - 1) % 7;

    seq_t result = (seq_t)761664;                  /* a(1)       */
    result += (seq_t)cycles * (seq_t)823543;       /* 823543 * cycles */
    result += INC_PREFIX[rem];                     /* remaining part */

    return result;
}
