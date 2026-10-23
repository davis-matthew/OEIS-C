/*
 * A124773  Number of permutations associated with compositions in standard
 *          order.
 * https://oeis.org/A124773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = number of permutations associated with the n‑th
 *            composition in the standard order (A066099)
 * Range:     u8 n <= 63, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types supplied by the build: */
#include <stdbool.h>

static inline seq_t factorial_small(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 1; i <= n; ++i)
        f *= i;
    return f;
}

static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;              /* exact division */
    }
    return res;
}

static inline seq_t product_of_composition(const int *parts, int len)
{
    seq_t prod = 1;
    for (int i = 0; i < len; ++i) {
        int rem_sum = 0;
        for (int j = i; j < len; ++j)
            rem_sum += parts[j];
        seq_t comb = binom_small((idx_t)rem_sum - 1, (idx_t)parts[i] - 1);
        seq_t fact = factorial_small((idx_t)parts[i] - 1);
        prod = prod * comb;
        prod = prod * fact;
    }
    return prod;
}

static void generate_desc(int remaining, int len, const int *parts,
                          idx_t target, idx_t *count, seq_t *result,
                          bool *found)
{
    if (*found)
        return;
    for (int part = remaining; part >= 1; --part) {
        int parts_array[64];
        if (len > 0)
            for (int i = 0; i < len; ++i)
                parts_array[i] = parts[i];
        parts_array[len] = part;
        if (remaining == part) {       /* leaf */
            if (*count == target) {
                *result = product_of_composition(parts_array, len + 1);
                *found = true;
                return;
            }
            ++(*count);
        } else {
            generate_desc(remaining - part, len + 1, parts_array,
                          target, count, result, found);
            if (*found)
                return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* empty composition */

    idx_t cumulative = 1;            /* count for an empty composition */
    for (int sum = 1; ; ++sum) {
        /* number of compositions of a given sum = 2^(sum-1) */
        idx_t num_comps = (idx_t)1 << (sum - 1);
        if (n < cumulative + num_comps) {
            idx_t target = n - cumulative;          /* 0‑based index */
            idx_t count = 0;
            seq_t result = 0;
            bool found = false;
            int parts_of_sum[64] = {0};
            generate_desc(sum, 0, parts_of_sum,
                          target, &count, &result, &found);
            return result;
        }
        cumulative += num_comps;
    }
}
