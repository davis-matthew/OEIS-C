/*
 * A168017  Triangle read by rows in which row n lists the number of
 *          partitions of n into parts divisible by d, where d is a divisor of
 *          n listed in decreasing order.
 * https://oeis.org/A168017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 175, u32 n <= 644, u64 n <= 2581,
 *            u128 n <= 9108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Triangle read row‑by‑row:  a(n) = p(r / d) where
 *  r = row that contains the n‑th term,
 *  d = the (pos‑th) largest divisor of r (pos is 0‑based within the row),
 *  p(k) = the number of unrestricted integer partitions of k.
 * 
 * The function is memoised and works for any of the builds
 * (fixed‑size unsigned or arbitrary‑precision integer classes). */
#include <stdlib.h>

/* Static arrays used for memoisation */
static seq_t *partitions = NULL;      /* partitions[0] = 1   */
static idx_t *cumulative = NULL;      /* cumulative[row] = total terms up to this row  */
static idx_t current_max_row = 0;     /* highest row pre‑computed   */
static idx_t capacity = 0;            /* allocated capacity (>= rows)      */

/* ------------------------------------------------------------------ */
/* Helper: compare idx_t for qsort descending */
static int cmp_desc(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Compute the number of divisors of n                                               */
static idx_t divisor_count(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d)
        if (n % d == 0)
        {
            ++cnt;
            if (d * d != n) ++cnt;
        }
    return cnt;
}

/* ------------------------------------------------------------------ */
/* Return the d‑th largest divisor of r (0‑based)                              */
static idx_t divisor_desc_at(idx_t r, idx_t pos)
{
    /* The number of divisors of r is small for r ≤ 10⁴ (≤ 128). */
    idx_t tmp[256];
    idx_t cnt = 0;
    for (idx_t d = 1; d * d <= r; ++d)
        if (r % d == 0)
        {
            tmp[cnt++] = d;
            if (d * d != r) tmp[cnt++] = r / d;
        }
    /* Sort in descending order. */
    qsort(tmp, cnt, sizeof(idx_t), cmp_desc);
    return tmp[pos];
}

/* ------------------------------------------------------------------ */
/* Ensure that partitions[0..row] and cumulative[0..row] are defined for
 * every row up to (and including) the smallest row with
 * cumulative[row] ≥ target_index.                                   */
static void precompute_until(idx_t target_index)
{
    /* Initialise the base case if this is the first call. */
    if (capacity == 0)
    {
        capacity = 16;
        partitions = (seq_t *)malloc(capacity * sizeof(seq_t));
        cumulative = (idx_t *)malloc(capacity * sizeof(idx_t));
        if (!partitions || !cumulative) abort();
        partitions[0] = (seq_t)1;   /* p(0) = 1 */
        cumulative[0] = (idx_t)0;   /* no terms before row 0 */
        current_max_row = 0;
    }

    /* Grow further rows until the required target_index falls within them. */
    while (current_max_row == 0 || cumulative[current_max_row] < target_index)
    {
        idx_t next = current_max_row + 1; /* row to compute */

        /* Ensure array capacity. */
        if (next >= capacity)
        {
            size_t new_cap = capacity ? capacity * 2 : 8;
            if (new_cap <= next) new_cap = next + 1;
            seq_t   *tmp_p = (seq_t *)realloc(partitions,   new_cap * sizeof(seq_t));
            idx_t   *tmp_c = (idx_t *)realloc(cumulative, new_cap * sizeof(idx_t));
            if (!tmp_p || !tmp_c) abort();
            partitions = tmp_p;
            cumulative = tmp_c;
            capacity   = new_cap;
        }

        /* Compute partition number p(next) using the pentagonal theorem. */
        seq_t sum = (seq_t)0;
        for (int k = 1; ; ++k)
        {
            idx_t k1 = (idx_t)k;
            idx_t pent1 = k1 * (3 * k1 - 1) / 2;
            if (pent1 > next) break;
            if (k & 1) sum += partitions[next - pent1];
            else       sum -= partitions[next - pent1];

            idx_t pent2 = k1 * (3 * k1 + 1) / 2;
            if (pent2 <= next)
            {
                if (k & 1) sum += partitions[next - pent2];
                else       sum -= partitions[next - pent2];
            }
        }
        partitions[next] = sum;

        /* Update cumulative terms count. */
        idx_t divcnt = divisor_count(next);
        cumulative[next] = cumulative[next - 1] + divcnt;
        current_max_row = next;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    precompute_until(n);

    /* Binary search for the row r containing the n‑th term. */
    idx_t lo = 1, hi = current_max_row, r = 1;
    while (lo <= hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (cumulative[mid] >= n)
        {
            r = mid;
            hi = mid - 1;
        }
        else
        {
            lo = mid + 1;
        }
    }

    idx_t prev_cum = cumulative[r - 1];
    idx_t pos = n - prev_cum - 1;         /* 0‑based index within row r */

    idx_t divisor = divisor_desc_at(r, pos);
    return partitions[r / divisor];
}
