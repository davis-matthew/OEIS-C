/*
 * A075155  Cubes of Lucas numbers.
 * https://oeis.org/A075155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000032(n)^3 (Lucas number cubed)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the Lucas number L(n) iteratively and returns L(n)^3. */
seq_t a(idx_t n)
{
    /* Base cases for the Lucas sequence and for the sequence itself */
    if (n == 0) return 8;      /* 2^3 = 8 */
    if (n == 1) return 1;      /* 1^3 = 1 */

    /* Compute Lucas number L(n) using L(0)=2, L(1)=1 */
    seq_t l0 = 2;   /* L(0) */
    seq_t l1 = 1;   /* L(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t l2 = l0 + l1;  /* L(i) = L(i-1) + L(i-2) */
        l0 = l1;
        l1 = l2;
    }
    seq_t l = l1;        /* Now l == L(n) */

    /* Return L(n)^3 */
    seq_t res = l * l;
    res = res * l;
    return res;
}
