/*
 * A105471  a(n) = Fibonacci(n) mod 100.
 * https://oeis.org/A105471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1) + a(n-2)) mod 100, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    static const seq_t MOD100 = 100;

    seq_t prev = 0;
    seq_t curr = 1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (prev + curr) % MOD100;
        prev = curr;
        curr = next;
    }

    return curr;
}
