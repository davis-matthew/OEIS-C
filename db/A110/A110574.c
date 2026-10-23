/*
 * A110574  Binary strings that have 1's where 'odious numbers' occur, 0's
 *          elsewhere and every term ends with the n-th odious number index.
 * https://oeis.org/A110574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the decimal number whose decimal digits form the binary
 *            string that has 1's exactly at positions that are odious
 *            numbers, up to the n‑th odious number. Thus for the n‑th odious
 *            number L, a(n) = Σ_{k=1..L} (is_odious(k)?1:0) * 10^{L-k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of odious numbers.  odious[i] contains the (i+1)-th odious number. */
static uint64_t *odious      = NULL;
static size_t   odious_cap   = 0;
static size_t   odious_cnt   = 0;

/* Ensure that at least n odious numbers are stored in the cache. */
static void ensure_odious(size_t n)
{
    if (odious_cnt >= n) return;           /* already have enough */

    size_t newcap = odious_cap ? odious_cap * 2 : 32;
    if (newcap < n) newcap = n;
    odious = (uint64_t *)realloc(odious, newcap * sizeof(uint64_t));
    odious_cap = newcap;

    uint64_t val = odious_cnt ? odious[odious_cnt-1] + 1 : 1;
    while (odious_cnt < n) {
        /* Skip non‑odious numbers.  A number is odious if it has
           an odd number of 1 bits in binary representation. */
        while (__builtin_popcountll(val) % 2 == 0) ++val;
        odious[odious_cnt++] = val;
        ++val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* padding for offset 1, harmless */

    ensure_odious((size_t)n);
    idx_t L = odious[(size_t)(n-1)];   /* n‑th odious number */

    seq_t res = 0;
    size_t od_ix = 0;                 /* index into odious array */

    for (idx_t pos = 1; pos <= L; ++pos) {
        int digit = 0;
        if (od_ix < n && odious[od_ix] == (uint64_t)pos) {
            digit = 1;
            ++od_ix;
        }
        res = res * 10 + (seq_t)digit;
    }
    return res;
}
