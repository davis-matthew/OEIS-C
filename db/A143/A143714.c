/*
 * A143714  Number of pairs (a,b), 1 <= a <= b <= n, such that (a+b)^2+n^2 is
 *          a square.
 * https://oeis.org/A143714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 533, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of unordered pairs (a,b) with 1 ≤ a ≤ b ≤ n
   such that (a+b)^2 + n^2 is a perfect square. */

#include <stddef.h>
#include <stdint.h>

/* Count pairs with a+b = s for given n (1 ≤ a ≤ b ≤ n). */
static seq_t
count_pairs(seq_t s, idx_t n)
{
    seq_t a_min = (s > (seq_t)n) ? (s - (seq_t)n) : (seq_t)1;
    seq_t a_max = s / (seq_t)2;
    if (a_max > (seq_t)n)
        a_max = (seq_t)n;
    return (a_max >= a_min) ? (a_max - a_min + (seq_t)1) : (seq_t)0;
}

/* Recursive generation of divisors of n^2 using the prime factorisation
   of n.  For each divisor u (≤ v), compute the contribution to
   total. */
static void
dfs(size_t idx, seq_t cur, const idx_t *primes, const idx_t *exps,
    size_t pc, seq_t n_squared, idx_t n, seq_t *total)
{
    if (idx == pc) {
        seq_t u = cur;
        /* Only consider u positive and not exceeding n.  If u > v
           the pair will be processed when u = v during recursion. */
        if (u == (seq_t)0 || u > (seq_t)n)
            return;
        seq_t v = n_squared / u;
        if (u > v)
            return;                       /* keep only one order */
        /* u and v must have the same parity. */
        if (((u ^ v) & (seq_t)1) == (seq_t)0) {
            seq_t s = (v - u) / (seq_t)2;   /* a+b */
            if (s >= (seq_t)2 && s <= (seq_t)2 * n)
                *total += count_pairs(s, n);
        }
        return;
    }
    idx_t p = primes[idx];
    idx_t e_max = exps[idx] * (idx_t)2; /* exponent in n^2 */
    seq_t val = (seq_t)1;
    for (idx_t e = (idx_t)0; e <= e_max; ++e) {
        dfs(idx + (size_t)1, cur * val, primes, exps, pc,
            n_squared, n, total);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)2)
        return (seq_t)0;  /* a(1)=0, a(2)=0 */

    /* Factor n. */
    idx_t tmp = n;
    idx_t primes[64];
    idx_t exps[64];
    size_t pc = (size_t)0;

    for (idx_t p = (idx_t)2; p * p <= tmp; ++p) {
        if (tmp % p == (idx_t)0) {
            idx_t e = (idx_t)0;
            while (tmp % p == (idx_t)0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > (idx_t)1) {
        primes[pc] = tmp;
        exps[pc] = (idx_t)1;
        ++pc;
    }

    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t total = (seq_t)0;
    dfs((size_t)0, (seq_t)1, primes, exps, pc, n_sq, n, &total);
    return total;
}
