/*
 * A067961  Number of binary arrangements without adjacent 1's on n X n torus
 *          connected n-s.
 * https://oeis.org/A067961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = L(n)^n, L(n) = Lucas number (A000032)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the nth Lucas number (L(0)=2, L(1)=1) */
    seq_t L0 = (seq_t)2;
    seq_t L1 = (seq_t)1;
    if (n == 0) return L0;          /* unused but keeps correctness */
    if (n == 1) return L1;          /* L(1) = 1 */
    seq_t prev = L0;                /* L(i-2) */
    seq_t curr = L1;                /* L(i-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + curr;   /* Lucas recurrence */
        prev = curr;
        curr = next;
    }
    /* Exponentiation by squaring: curr = L(n) */
    seq_t result = (seq_t)1;
    seq_t base = curr;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
