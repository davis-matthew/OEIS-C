/*
 * A216599  Number of ways to write n as a product with all factors <= n/3.
 * https://oeis.org/A216599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multiplicative partitions of n with all factors
 *            not exceeding floor(n/3). Each factor is ≥2 and the factor list
 *            is taken non‑decreasing so that each product is counted exactly
 *            once
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_factorizations(idx_t n, idx_t min_f, idx_t max_f)
{
    seq_t total = 0;

    /* If the remaining product is smaller than the smallest allowed factor,
       no further factors can be chosen. */
    if (n < min_f)
        return 0;

    /* Enumerate all admissible factors of n, starting from min_f and not
       exceeding either max_f or n itself. */
    for (idx_t d = min_f; d <= max_f && d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t rest = n / d;
        /* enforce non‑decreasing order: the next factor must be at least d */
        if (rest < d)
            continue;

        /* If the complementary factor rest is also ≤ max_f, then
           the pair (d,rest) constitutes a valid factorization. */
        if (rest <= max_f)
            total += 1;

        /* Recurse on the remaining part rest, keeping the lower bound d
           to preserve ordering. */
        total += count_factorizations(rest, d, max_f);
    }

    return total;
}

seq_t a(idx_t n)
{
    /* For n < 2 there are no non‑trivial factorizations. */
    if (n < 2)
        return 0;

    /* All factors must be at most floor(n/3). */
    idx_t limit = n / 3;
    if (limit < 2)
        return 0;

    return count_factorizations(n, 2, limit);
}
