/*
 * A048669  The Jacobsthal function g(n): maximal gap in a list of all the
 *          integers relatively prime to n.
 * https://oeis.org/A048669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = maximum gap between consecutive integers that are coprime
 *            to n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(1) = 1 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n <= 1) return (seq_t)1;

    /* Factor n and collect its distinct prime divisors.  The number of
       distinct primes for n ≤ 10⁴ is at most 5, so a small static array is
       sufficient. */
    uint64_t rad = 1;                 /* rad(n) = product of distinct primes */
    uint64_t primes[16];
    size_t   pcnt = 0;
    uint64_t m = (uint64_t)n;

    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            rad *= d;
            primes[pcnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) {                      /* remaining prime factor */
        rad *= m;
        primes[pcnt++] = m;
    }

    /* Scan residues modulo rad to find the largest distance between
       successive residues that are coprime to n. */
    uint64_t last  = (uint64_t)-1;
    uint64_t first = (uint64_t)-1;
    uint64_t max_gap = 0;

    for (uint64_t r = 0; r < rad; ++r) {
        bool coprime = true;
        for (size_t i = 0; i < pcnt; ++i) {
            if (r % primes[i] == 0) { coprime = false; break; }
        }
        if (coprime) {
            if (first == (uint64_t)-1) first = r;
            if (last != (uint64_t)-1) {
                uint64_t diff = r - last;
                if (diff > max_gap) max_gap = diff;
            }
            last = r;
        }
    }

    /* Wrap‑around gap from the last to the first coprime residue */
    if (last != (uint64_t)-1 && first != (uint64_t)-1) {
        uint64_t wrap = rad - last + first;
        if (wrap > max_gap) max_gap = wrap;
    }

    return (seq_t)max_gap;
}
