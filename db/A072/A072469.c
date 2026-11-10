/*
 * A072469  Right diagonal of triangle in A072467.
 * https://oeis.org/A072469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first term of row n of the triangle A072467 (row n holds
 *            n consecutive primes == 1 mod n, each row starting after the
 *            previous row's last term); computed by a direct search
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A072469_MAXROWS 2048

static uint64_t A072469_first[A072469_MAXROWS + 1];
static uint64_t A072469_rows = 0;
static uint64_t A072469_last = 0;

static bool A072469_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1ULL) == 0) return false;
    if (x % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= x; d += 6) {
        if (x % d == 0 || x % (d + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A072469_MAXROWS) return 0;
    while (A072469_rows < n) {
        uint64_t r = A072469_rows + 1;
        uint64_t c = (A072469_last / r) * r + 1;
        if (c <= A072469_last) c += r;
        uint64_t found = 0;
        for (;; c += r) {
            if (A072469_isprime(c)) {
                if (found == 0) A072469_first[r] = c;
                found++;
                A072469_last = c;
                if (found == r) break;
            }
        }
        A072469_rows = r;
    }
    return (seq_t)A072469_first[n];
}
