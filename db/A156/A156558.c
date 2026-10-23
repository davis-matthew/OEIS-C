/*
 * A156558  a(n) = Sum_{k=1..n} (n^k mod (n-k+1)).
 * https://oeis.org/A156558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (n^k mod (n-k+1)) with a(0)=0
 * Range:     u8 n <= 37, u16 n <= 530, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute (base^exp) mod mod for non‑negative exp.
   All values are small (≤10000); use 64‑bit unsigned arithmetic
   which suffices for all supported seq_t types. */
static inline uint64_t powmod_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t sum = 0;
    uint64_t n64 = (uint64_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t mod = n - k + 1;           /* mod ≥ 1 */
        uint64_t term = powmod_uint64(n64, k, mod);
        sum += term;
    }

    return (seq_t)sum;
}
