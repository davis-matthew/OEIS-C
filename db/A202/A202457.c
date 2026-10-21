/*
 * A202457  Number of (n+2) X 6 binary arrays with consecutive windows of
 *          three bits considered as a binary number nondecreasing in every
 *          row and column.
 * https://oeis.org/A202457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 51*n^5 + 810*n^4 + 5975*n^3 + 22769*n^2 + 56554*n
 *            + 73560) / 120
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the empirical polynomial expression found for A202457. */

/* Computes a(n) for n >= 1 using 64‑bit or arbitrary‑precision arithmetic. */
seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t tn = (seq_t)n;

    /* Compute successive powers of n: n, n^2, n^3, …, n^6. */
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t n3 = n2 * tn;          /* n^3 */
    seq_t n4 = n3 * tn;          /* n^4 */
    seq_t n5 = n4 * tn;          /* n^5 */
    seq_t n6 = n5 * tn;          /* n^6 */

    /* Combine terms with their coefficients. */
    seq_t num = n6;                          /* n^6 / 120 */
    num += (seq_t)51 * n5;                   /* 51*n^5 / 120 */
    num += (seq_t)810 * n4;                  /* 810*n^4 / 120 */
    num += (seq_t)5975 * n3;                 /* 5975*n^3 / 120 */
    num += (seq_t)22769 * n2;                /* 22769*n^2 / 120 */
    num += (seq_t)56554 * tn;                /* 56554*n / 120 */
    num += (seq_t)73560;                     /* 73560 / 120 */

    /* Exact division by 120 (120 = 2^3 * 3 * 5). */
    return num / (seq_t)120;
}
