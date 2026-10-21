/*
 * A088737  Number of semiprime divisors of n-th composite number.
 * https://oeis.org/A088737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056170(n) + A001221(n)*(A001221(n)-1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_counts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of semiprime divisors of the n‑th composite number */

#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool */
#include "../utils/is_prime.h"
#include "../utils/factor_counts.h"

static idx_t *composites = NULL;      /* array of composites */
static size_t comp_cap   = 0;         /* current capacity of composites array */
static idx_t composite_count = 0;    /* number of composites stored */

static idx_t last_checked = 3;       /* last integer examined for compositeness */

/* Ensure the composites array can hold at least 'needed' elements */
static void ensure_composite_capacity(size_t needed)
{
    if (needed <= comp_cap)
        return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    composites = (idx_t *)realloc(composites, new_cap * sizeof(idx_t));
    comp_cap = new_cap;
}

/* Lazily generate composites until at least 'n' composites are stored */
static void generate_composites(idx_t n)
{
    if (n <= composite_count)
        return;

    ensure_composite_capacity((size_t)n);

    while (composite_count < n) {
        ++last_checked;
        if (!is_prime(last_checked)) {
            composites[composite_count++] = last_checked;
        }
    }
}

/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset 1; behaviour for n<1 is unspecified */
        return 0;

    generate_composites(n);
    uint64_t composite = composites[n - 1];

    seq_t distinct, square;
    factor_counts(composite, &distinct, &square);

    seq_t result = square + distinct * (distinct - 1) / 2;
    return result;
}
