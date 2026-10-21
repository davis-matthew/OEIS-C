/*
 * A215851  Number of simple labeled graphs on n nodes with exactly 1
 *          connected component that is a tree or a cycle.
 * https://oeis.org/A215851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(n)=n^(n-2)+(n-1)!/2 for n>2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp) */
#include "../utils/factorial.h"   /* factorial(idx_t n) */

static seq_t tree_component_count(idx_t n)
{
    /* n^(n-2) */
    return big_pow((seq_t)n, (seq_t)(n - 2));
}

static seq_t cycle_component_count(idx_t n)
{
    /* (n-1)! / 2 */
    return factorial(n - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return 1;
    return tree_component_count(n) + cycle_component_count(n);
}
