/*
 * A097092  Number of partitions of n such that the least part occurs exactly
 *          four times.
 * https://oeis.org/A097092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -p(n) + 4*p(n+4) - p(n+5) - 2*p(n+6) - 2*p(n+7) + p(n+8)
 *            + 2*p(n+9) - p(n+10) where p(k) is the partition number of k
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 153, u64 n <= 467,
 *            u128 n <= 1567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>


#define PART_MAX 20020               /* enough for n up to about 10000 */

static seq_t part[PART_MAX] = {1};   /* part[0] = 1, others 0 */
static idx_t part_computed = 1;      /* number of partition values already computed (up to part[part_computed-1]) */

/* Compute partition numbers up to and including limit using the
   generalized pentagonal number recurrence. */
static void ensure_partitions(idx_t limit)
{
    if (part_computed > limit) return;          /* nothing to do */

    for (idx_t i = part_computed; i <= limit; ++i)
    {
        seq_t sum = 0;
        for (int k = 1; ; ++k)
        {
            idx_t pent1 = (idx_t)k * (3 * (idx_t)k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = part[i - pent1];
            if (k & 1) sum += term;      /* k odd -> + */
            else       sum -= term;      /* k even -> - */

            idx_t pent2 = (idx_t)k * (3 * (idx_t)k + 1) / 2;
            if (pent2 > i) continue;
            term = part[i - pent2];
            if (k & 1) sum += term;
            else       sum -= term;
        }
        part[i] = sum;
    }
    part_computed = limit + 1;
}

seq_t a(idx_t n)
{
    /* We need partition numbers up to n+10. */
    idx_t need = n + 10;
    if (need >= PART_MAX) need = PART_MAX - 1;   /* defensive, should not happen */
    ensure_partitions(need);

    /* Compute the linear combination.  All intermediate values are non‑negative. */
    seq_t pos = 0;
    pos += 4 * part[n + 4];
    pos += part[n + 8];
    pos += 2 * part[n + 9];

    seq_t neg = 0;
    neg += part[n];
    neg += part[n + 5];
    neg += 2 * part[n + 6];
    neg += 2 * part[n + 7];
    neg += part[n + 10];

    return pos - neg;   /* guaranteed non‑negative for n >= 1 */
}
