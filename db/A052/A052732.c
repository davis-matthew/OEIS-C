/*
 * A052732  E.g.f.: (1-2x-sqrt(1-4*x))*x^2/2.
 * https://oeis.org/A052732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A052732
   Formula: a(n) = n! * C_{n-3}  for n > 3,  C_k = (2k)!/(k!(k+1)!)
   with a(0)=a(1)=a(2)=a(3)=0.
   The function returns a(n) exactly when it fits in seq_t; for the 64‑bit
   build this holds up to n = 16.  Intermediate arithmetic uses 128‑bit
   unsigned integers so that no overflow occurs for the values that must
   be correct. */
seq_t a(idx_t n)
{
    /* base cases, matching OEIS offset 0 */
    if (n <= 3)
        return 0;

    /* Catalan index k = n-3 */
    idx_t k = n - 3;

    /* compute binomial(2k, k) = (2k)!/(k!k!) multiplicatively.
       Use 128‑bit to keep intermediate safe. */
    __uint128_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * (k + i) / i;   /* exact division at each step */
    }

    /* factorial n! */
    __uint128_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= i;
    }

    /* a(n) = n! * binom / (n-2)   (since (n-2) = k+1) */
    __uint128_t res = fact * binom / (n - 2);

    return (seq_t)res;
}
