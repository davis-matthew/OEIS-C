/*
 * A088808  Number of subsets of {1, ..., n} that are not double-free.
 * https://oeis.org/A088808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n – Π_{odd m ≤ n} Fib(L_m+2) where L_m = ⌊log₂(n/m)⌋ +
 *            1 is the length of the chain produced by repeatedly doubling the
 *            odd number m. Fib(k) denotes the Fibonacci numbers with
 *            Fib(0)=0, Fib(1)=1. This counts the subsets of {1,…,n} that
 *            contain at least one pair {x,2x}
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* maximum chain length for odd starting elements */
    idx_t tmp = n;
    idx_t maxLen = 0;
    while (tmp > 0) { tmp >>= 1; maxLen++; }   /* maxLen = floor_log2(n)+1 */

    /* precompute Fibonacci numbers up to Fib(maxLen+2) */
    /* Fib(0)=0, Fib(1)=1 */
    static seq_t fib[64];
    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i <= maxLen + 2; ++i)
        fib[i] = fib[i-1] + fib[i-2];

    /* count double‑free subsets: product of independent‑set counts over chains */
    seq_t doubleFree = 1;
    for (idx_t m = 1; m <= n; m += 2) {
        /* compute the length L_m of the chain starting from m */
        idx_t len = 0;
        idx_t val = m;
        while (val <= n) {
            ++len;
            val <<= 1;          /* multiply by 2 using shift on idx_t */
        }
        doubleFree *= fib[len + 2];
    }

    /* total number of subsets: 2^n */
    seq_t pow2 = 1;
    seq_t two = 2;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= two;

    return pow2 - doubleFree;
}
