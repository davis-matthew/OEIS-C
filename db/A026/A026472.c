/*
 * A026472  {3, 7} together with the numbers congruent to {1, 2} mod 12.
 * https://oeis.org/A026472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=7; for n≥5 let k =
 *            floor((n-5)/2)+1, r = (n-5) mod 2; then a(n) = 12*k+1 if r==0
 *            else 12*k+2. This is the set {3,7} together with all integers ≡1
 *            or 2 (mod 12)
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)n;
    if (n == 3) return 3;
    if (n == 4) return 7;
    /* n ≥ 5 */
    idx_t idx = n - 5;                 /* idx = n-5 */
    seq_t k = ((seq_t)(idx >> 1)) + 1; /* k = floor(idx/2)+1 */
    seq_t r = (seq_t)(idx & 1);        /* r = idx mod 2 */
    if (r == 0)
        return ((seq_t)12) * k + 1;
    else
        return ((seq_t)12) * k + 2;
}
