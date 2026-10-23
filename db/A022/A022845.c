/*
 * A022845  Expansion of 1/((1-x)*(1-5*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A022845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A022845  –  Expansion of 1/((1-x)*(1-5*x)*(1-9*x)*(1-10*x))
 *
 *  Recurrence (valid for n ≥ 4):
 *      a(n) = 25*a(n-1) - 209*a(n-2) + 635*a(n-3) - 450*a(n-4)
 *  Initial values:
 *      a(0) = 1,  a(1) = 25,  a(2) = 416,  a(3) = 5810
 *
 *  The function computes a(n) for any n ≥ 0 using unsigned integer
 *  arithmetic only (seq_t may be 8, 16, 32, 64 or 128 bit, or an
 *  arbitrary‑precision type).  All operations are performed in
 *  seq_t; no temporary signed intermediate is used.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 1;   /* a(0)  */
    seq_t a1 = 25;  /* a(1)  */
    seq_t a2 = 416; /* a(2)  */
    seq_t a3 = 5810;/* a(3)  */

    /* Handle the initial indices directly. */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Iteratively compute up to the requested n. */
    for (idx_t i = 4; i <= n; ++i) {
        /*   next = 25*a3 - 209*a2 + 635*a1 - 450*a0
         * Order the operations to keep intermediate results
         * non‑negative (all terms are unsigned). */
        seq_t t1 = a3 * (seq_t)25;   /* 25*a(n-1)   */
        seq_t t2 = a2 * (seq_t)209;  /* 209*a(n-2)  */
        seq_t t3 = a1 * (seq_t)635;  /* 635*a(n-3)  */
        seq_t t4 = a0 * (seq_t)450;  /* 450*a(n-4)  */

        /* Compute next = t1 - t2 + t3 - t4 safely. */
        seq_t next = t1 + t3;        /* (25*a3 + 635*a1)  */
        next -= t2;                  /* - 209*a2           */
        next -= t4;                  /* - 450*a0           */

        /* Shift window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;   /* a(n) */
}
