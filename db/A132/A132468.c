/*
 * A132468  Longest gap between numbers relatively prime to n.
 * https://oeis.org/A132468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8118, u16 n <= 8118, u32 n <= 8118, u64 n <= 8118,
 *            u128 n <= 8118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = maximum length of a run of consecutive integers
   that are NOT coprime to n, i.e., the largest gap between
   successive integers that are coprime to n (endpoints excluded). */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Collect distinct prime factors of n. */
    #define MAX_PRIMES 64
    idx_t primes[MAX_PRIMES];
    size_t pfCnt = 0;

    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            primes[pfCnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
            if (pfCnt == MAX_PRIMES) /* safety, though unlikely for n≤10⁴ */
                break;
        }
    }
    if (tmp > 1 && pfCnt < MAX_PRIMES)
        primes[pfCnt++] = tmp;

    /* Scan residues 1 .. n-1 to find coprime gaps. */
    idx_t maxgap = 0;
    idx_t first = 0, last = 0;
    bool first_set = false;

    for (idx_t r = 1; r < n; ++r) {
        bool coprime = true;
        for (size_t i = 0; i < pfCnt; ++i) {
            if (r % primes[i] == 0) {
                coprime = false;
                break;
            }
        }
        if (!coprime)
            continue;

        if (!first_set) {
            first = r;
            first_set = true;
        }
        if (last != 0) {
            idx_t gap = r - last - 1;
            if (gap > maxgap)
                maxgap = gap;
        }
        last = r;
    }

    /* Wrap‑around gap from last to first (include n itself). */
    idx_t wrap_gap = (n - last) + (first - 1);
    if (wrap_gap > maxgap)
        maxgap = wrap_gap;

    return (seq_t)maxgap;
}
