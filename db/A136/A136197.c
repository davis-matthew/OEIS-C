/*
 * A136197  Fixed points or cycle values for sequences of the form
 *          s(k+1)=sopfr(2s(k)+1), with sopfr=A001414 and any s(1).
 * https://oeis.org/A136197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed points or cycle values for s(k+1)=sopfr(2s(k)+1) where sopfr is the sum of
 * prime factors with multiplicity (A001414).  The sequence a(n) lists the
 * distinct integers that eventually appear as fixed points or elements of a
 * cycle under this recurrence, sorted increasingly.  The function below
 * generates them on demand using cycle detection and keeps a static set of
 * discovered values.  It returns the n‑th value for all n>=1, defining 0
 * for indices beyond the set (no further values are known). */

#include <stdint.h>
#include <stddef.h>

/* Helper: sum of prime factors (with multiplicity) of a 64‑bit integer.
 * It is used in the recurrence step: sopfr(2*x+1).  Numbers produced in this
 * sequence are always small, so simple trial division suffices. */
static seq_t sopfr_uint64(uint64_t n)
{
    seq_t sum = 0;
    while ((n & 1ULL) == 0ULL) { sum += 2; n >>= 1; }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0ULL) { sum += (seq_t)p; n /= p; }
    }
    if (n > 1ULL) sum += (seq_t)n;
    return sum;
}

/* One step of the recurrence: s_{k+1} = sopfr(2*s_k + 1) */
static seq_t step_seq(seq_t x)
{
    uint64_t val = ((uint64_t)2UL) * (uint64_t)x + 1UL;
    return sopfr_uint64(val);
}

/* static storage for discovered cycle / fixed‑point values, kept in sorted order */
static seq_t known_values[128];
static size_t known_count = 0;

/* insert a value while keeping known_values sorted and unique */
static void insert_known(seq_t v)
{
    for (size_t i = 0; i < known_count; ++i) {
        if (known_values[i] == v) return;                /* already present */
        if (v < known_values[i]) {
            /* shift elements to make room */
            for (size_t j = known_count; j > i; --j) known_values[j] = known_values[j - 1];
            known_values[i] = v;
            ++known_count;
            return;
        }
    }
    known_values[known_count++] = v;                       /* append at end */
}

/* collect all distinct values of the cycle (including fixed point) that the
 * sequence starting at 'start' eventually reaches.  The values are passed
 * to the callback, which in our case inserts them into the global list. */
static void collect_cycle(seq_t start, void (*callback)(seq_t))
{
    /* Floyd’s cycle‑finding algorithm */
    seq_t tort = step_seq(start);
    seq_t hare = step_seq(step_seq(start));
    while (tort != hare) {
        tort = step_seq(tort);
        hare = step_seq(step_seq(hare));
    }

    /* find entry point of the cycle */
    tort = start;
    while (tort != hare) {
        tort = step_seq(tort);
        hare = step_seq(hare);
    }
    seq_t cycle_start = tort;

    /* walk around the cycle once, sending each value to the callback */
    seq_t cur = cycle_start;
    do {
        callback(cur);
        cur = step_seq(cur);
    } while (cur != cycle_start);
}

/* next starting value to try when filling known_values */
static idx_t next_start = 1;

seq_t a(idx_t n)
{
    /* generate enough distinct values to answer this query
       (stop once we have at least n values or we have tried a lot
        of starting positions that produce no new values) */
    const idx_t MAX_START = 200000;           /* safe large limit */
    const idx_t NO_NEW_LIMIT = 500;           /* stop if many starts give nothing new */

    static idx_t no_new_count = 0;

    while (known_count < n) {
        /* try the next starting value; if we exceed the safety limit, give up */
        if (next_start > MAX_START) break;

        /* collect cycle nodes for this start */
        size_t before = known_count;
        collect_cycle((seq_t)next_start, insert_known);

        if (known_count == before) ++no_new_count;
        else no_new_count = 0;           /* we got at least one new value */

        ++next_start;

        /* if we have been unable to find new values for a while, stop */
        if (no_new_count >= NO_NEW_LIMIT) break;
    }

    if (n == 0 || n > known_count) return (seq_t)0;
    return known_values[n - 1];
}
