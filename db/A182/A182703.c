/*
 * A182703  Triangle read by rows: T(n,k) = number of occurrences of k in the
 *          last section of the set of partitions of n.
 * https://oeis.org/A182703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   Let r = ceil((sqrt(8n+1)-1)/2) and * k = n – r(r-1)/2. Denote
 *            p(m)=number of partitions of m, * U(m,k)=U(m-k,k)+p(m-k) with
 *            U(0,k)=0. * Then a(n) = p(r-1) if k==1, otherwise
 *            U(r,k)-U(r-1,k)
 * Range:     u8 n <= 153, u16 n <= 990, u32 n <= 8256, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* find the smallest r such that r*(r+1)/2 >= n */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

/* static cache for partition numbers */
static seq_t *partitions = NULL;
static idx_t max_pre = 0;

/* recompute partition numbers up to limit (inclusive) */
static void recompute_partitions(idx_t limit)
{
    if (limit <= max_pre) return;
    partitions = (seq_t*)realloc(partitions, (limit + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= limit; ++i) partitions[i] = 0;
    partitions[0] = 1;
    for (idx_t part = 1; part <= limit; ++part) {
        for (idx_t sum = part; sum <= limit; ++sum) {
            partitions[sum] += partitions[sum - part];
        }
    }
    max_pre = limit;
}

/* count occurrences of part k in all partitions of m   */
static seq_t count_occurrences(idx_t m, idx_t k)
{
    seq_t acc = 0;
    for (idx_t cur = m; cur >= k; cur -= k) {
        acc += partitions[cur - k];
    }
    return acc;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, safe guard */

    idx_t r = find_row(n);
    idx_t tri_before = (r - 1) * r / 2;
    idx_t k = n - tri_before;   /* column (1‑based) */

    /* we need partition numbers up to r-1 and r            */
    recompute_partitions(r);

    if (k == 1) {
        return partitions[r - 1];
    } else {
        seq_t u1 = count_occurrences(r, k);
        seq_t u2 = (r > 0) ? count_occurrences(r - 1, k) : 0;
        return u1 - u2;
    }
}
