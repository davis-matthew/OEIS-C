/*
 * A096737  a(1) = 2; for n>1: a(n) = integer part of y-value when x=0 in
 *          (y-tau(n))/(x-1)=(1-tau(n))/(n-1), tau=A000005.
 * https://oeis.org/A096737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(2) = 3; a(n) = τ(n) for n > 2 where τ(n) is the
 *            number of positive divisors of n (A000005)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of a 64‑bit integer n. */
static seq_t divisor_count(idx_t n) {
    idx_t m = n;
    seq_t count = 1;
    unsigned int exp = 0;

    /* Count factor 2. */
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp)
        count *= (seq_t)(exp + 1);

    /* Count odd factors. */
    for (idx_t p = 3; p <= m / p; p += 2) {
        exp = 0;
        while (m % p == 0) {
            exp++;
            m /= p;
        }
        if (exp)
            count *= (seq_t)(exp + 1);
    }

    /* If m > 1, it is a prime factor > sqrt(original n). */
    if (m > 1)
        count *= 2;

    return count;
}

seq_t a(idx_t n) {
    if (n == 1) return 2;
    if (n == 2) return 3;
    /* For n > 2, a(n) = τ(n). */
    return divisor_count(n);
}
