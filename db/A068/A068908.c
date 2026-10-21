/*
 * A068908  Number of partitions of n modulo 5.
 * https://oeis.org/A068908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n modulo 5. Computed via Euler's
 *            recurrence: a(0) = 1 a(n) = Σ_{k≠0} (-1)^{k-1} a(n - k(3k-1)/2)
 *            (mod 5), where the sum is over positive k and k(3k±1)/2 ≤ n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *part_mod5 = NULL;   /* cache of p(n) mod 5 */
static idx_t  max_n_done = 0;     /* highest n for which cache is filled */

/* Ensure cache is allocated for n = 0. */
static void init_cache(void)
{
    if (!part_mod5)
    {
        part_mod5 = (seq_t *)malloc(sizeof(seq_t));
        if (!part_mod5) exit(EXIT_FAILURE);
        part_mod5[0] = (seq_t)1;   /* p(0) = 1 */
        max_n_done = 0;
    }
}

/* Extend the cache up to at least n, using Euler's recurrence. */
static void compute_up_to(idx_t n)
{
    init_cache();

    if (n <= max_n_done) return;

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(part_mod5, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    part_mod5 = tmp;

    for (idx_t i = max_n_done + 1; i <= n; ++i)
    {
        /* i > 0: compute part_mod5[i] via the pentagonal‑number recurrence */
        int sum = 0;                 /* temporary signed integer sum */
        for (idx_t k = 1; ; ++k)
        {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;

            int sign = (k & 1) ? 1 : -1;          /* (-1)^(k+1) */
            sum += sign * (int)part_mod5[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i)
                sum += sign * (int)part_mod5[i - pent2];
        }
        int mod = sum % 5;
        if (mod < 0) mod += 5;   /* bring to 0..4 */
        part_mod5[i] = (seq_t)mod;
    }
    max_n_done = n;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return part_mod5[n];
}
