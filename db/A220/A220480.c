/*
 * A220480  The number of n-digit numbers where last k digits are divisible by
 *          the k-th prime for k = 1..n.
 * https://oeis.org/A220480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number of n‑digit integers whose last k digits are
 *            divisible by the k‑th prime (k = 1..n). Computed by exhaustive
 *            search built from the units digit outward, imposing the
 *            divisibility constraints at each step. For n > 9 there are no
 *            solutions
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* first nine primes needed for n ≤ 9 */
static const uint32_t primes[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};

/* depth‑first search from the least significant digit upward.
   pos      – number of digits already fixed (0 … target).
   rem[]    – remainders of the current suffix modulo each prime.
   pow10[]  – 10^pos modulo each prime (used to extend remainders).
   target   – desired number of digits (n). */
static seq_t dfs(int pos, uint32_t rem[9], uint32_t pow10[9], int target)
{
    if (pos == target)
        return (seq_t)1;  /* a complete n‑digit number has been built */

    seq_t total = 0;
    for (int d = 0; d <= 9; ++d) {
        /* avoid leading zero when this is the most significant digit
           (except for n == 1, where 0 is allowed). */
        if (pos == target - 1 && d == 0 && target > 1)
            continue;

        uint32_t new_rem[9];
        uint32_t new_pow10[9];
        for (int k = 0; k < target; ++k) {
            /* extend remainder for p_k and update pow10 for next level */
            new_rem[k] = (rem[k] + (uint32_t)d * pow10[k]) % primes[k];
            new_pow10[k] = (pow10[k] * 10u) % primes[k];
        }

        /* The (pos+1)‑digit suffix must be divisible by p_pos. */
        if (new_rem[pos] == 0)
            total += dfs(pos + 1, new_rem, new_pow10, target);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* offset 1 – undefined for n = 0 */
        return (seq_t)0;
    if (n > 9)                /* no solutions beyond 9 digits */
        return (seq_t)0;

    /* memoise results for n = 1 … 9 */
    static bool seen[10] = {false};
    static seq_t cache[10] = {0};

    int ni = (int)n;          /* safe, since n ≤ 9 */
    if (!seen[ni]) {
        uint32_t rem[9]   = {0};
        uint32_t pow10[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; /* 10^0 mod primes */
        cache[ni] = dfs(0, rem, pow10, ni);
        seen[ni]  = true;
    }
    return cache[ni];
}
