/*
 * A167321  Totally multiplicative sequence with a(p) = 2*(p+3) for prime p.
 * https://oeis.org/A167321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (2*(p+3))^e ; a(1)=1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t num = n;                 /* working copy of n */

    /* factor 2 */
    idx_t exp = 0;
    while ((num & 1) == 0) {
        exp++;
        num >>= 1;
    }
    if (exp > 0) {
        seq_t base = (seq_t)2 * ((seq_t)2 + 3);   /* (2*(2+3)) = 10 */
        seq_t pow  = 1;
        for (idx_t k = 0; k < exp; ++k) pow *= base;
        result *= pow;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= num / p; p += 2) {
        if (num % p != 0) continue;
        exp = 0;
        while (num % p == 0) {
            exp++;
            num /= p;
        }
        seq_t base = (seq_t)2 * ((seq_t)p + 3);    /* (2*(p+3)) */
        seq_t pow  = 1;
        for (idx_t k = 0; k < exp; ++k) pow *= base;
        result *= pow;
    }

    /* remaining prime factor > sqrt(original n) */
    if (num > 1) {
        seq_t base = (seq_t)2 * ((seq_t)num + 3);
        result *= base;
    }

    return result;
}
