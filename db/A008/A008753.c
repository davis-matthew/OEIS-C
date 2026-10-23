/*
 * A008753  Expansion of (1+x^10)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 629, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (valid for n ≥ 5):
   a(n) = (6*(n-2)^2 + 143 + 4*cos(2*n*π/3) + 4*√3*sin(2*n*π/3) + 9*(-1)^n) / 36.
   For n < 5 the sequence is defined by: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=4.
   This implementation evaluates the closed‑form in integer arithmetic. */

seq_t a(idx_t n) {
    /* Initial values for n = 0 … 4 */
    static const seq_t init[5] = {1, 1, 2, 3, 4};
    if (n < 5) return init[n];

    /* Compute the closed form.  All intermediate arithmetic is with
       signed long long, which is safe for n up to millions (the test
       harness uses n ≤ 10 000). */
    long long nn = (long long)n;                     /* convert to signed type */
    long long sq = (nn - 2) * (nn - 2);              /* (n-2)^2 */
    long long numer = 6 * sq + 143;                  /* 6*(n-2)^2 + 143 */

    /* cos(2*n*π/3) + √3*sin(2*n*π/3) term: cycle 4 for n mod 6 ≠ 2,5
       and -8 for n mod 6 == 2 or 5. */
    uint64_t r = n % 6;
    long long cos_term = (r == 2 || r == 5) ? -8 : 4;
    numer += cos_term;

    /* 9*(-1)^n : add 9 for even n, subtract 9 for odd n. */
    numer += (n & 1ULL) ? -9 : 9;

    /* Integer division – the numerator is always divisible by 36. */
    return (seq_t)(numer / 36);
}
