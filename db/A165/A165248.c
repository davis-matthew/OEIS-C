/*
 * A165248  Quintisection A061037(5*n+2).
 * https://oeis.org/A165248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A165248: Quintisection A061037(5*n+2)
   Formula (piecewise by n mod 4):
   if n % 4 == 0: a(n) = 5 * (n/4) * (5*(n/4) + 1)
   if n % 4 == 1 or 3: a(n) = 5 * n * (5*n + 4)
   if n % 4 == 2: a(n) = 5 * (2*((n-2)/4) + 1) * (10*((n-2)/4) + 7)
   This implements the conjectured recurrence and explicit closed form.
   Complexity: O(1) per call.
*/
seq_t a(idx_t n)
{
    idx_t r = n % 4;            /* remainder of n modulo 4 */

    if (r == 0) {
        idx_t k = n / 4;        /* k = n/4 */
        seq_t K = (seq_t)k;
        return ((seq_t)5) * K * (((seq_t)5) * K + (seq_t)1);
    } else if (r == 1 || r == 3) {
        seq_t N = (seq_t)n;
        return ((seq_t)5) * N * (((seq_t)5) * N + (seq_t)4);
    } else {                    /* r == 2 */
        idx_t k = (n - 2) / 4;  /* k = (n-2)/4, valid for n ≡ 2 (mod 4) */
        seq_t K = (seq_t)k;
        seq_t term1 = ((seq_t)2) * K + (seq_t)1;
        seq_t term2 = ((seq_t)10) * K + (seq_t)7;
        return ((seq_t)5) * term1 * term2;
    }
}
