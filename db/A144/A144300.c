/*
 * A144300  Number of partitions of n minus number of divisors of n.
 * https://oeis.org/A144300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000041(n) - A000005(n) (Partitions – Divisor counts)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for partitions and divisor counts */
static idx_t part_max = 0, part_cap = 0;
static seq_t *partitions = NULL;

static idx_t div_max = 0, div_cap = 0;
static seq_t *divcnt = NULL;

static void ensure_partitions(idx_t n)
{
    if (n <= part_max) return;                 /* already have up to n */

    /* allocate new storage */
    seq_t *new_part = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!new_part) return;                     /* allocation failure */

    new_part[0] = 1;                           /* p(0) = 1 */

    /* compute partitions up to n using the standard generating function */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            new_part[j] += new_part[j - i];
        }
    }

    /* replace old array */
    free(partitions);
    partitions = new_part;
    part_max   = n;
}

static void ensure_divisors(idx_t n)
{
    if (n <= div_max) return;                 /* already computed */

    seq_t *new_div = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!new_div) return;                     /* allocation failure */

    /* sieve-like computation of divisor counts */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            new_div[j] += 1;
        }
    }

    /* replace old array */
    free(divcnt);
    divcnt = new_div;
    div_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of defined range */

    ensure_partitions(n);
    ensure_divisors(n);

    return partitions[n] - divcnt[n];
}
