/*
 * A213676  Reversed Zeckendorf binary representation of natural numbers.
 * https://oeis.org/A213676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth bit (0‑based) of the infinite binary word
 *            obtained by concatenating, in order of natural numbers, the
 *            Zeckendorf binary representations written with least‑significant
 *            bit first. (Each representation is the unique binary string of
 *            0/1 such that n = Σ bit_i * F_{i+2} where F_0=1, F_...
 * Range:     u8 n <= 5752, u16 n <= 9695, u32 n <= 9695, u64 n <= 9695,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_FIB 2000          /* enough for all practical n */

seq_t a(idx_t n)
{
    /* Pre‑computed Fibonacci numbers (F_0=1, F_1=2, F_2=3, …) */
    static seq_t fib[MAX_FIB];
    static size_t fib_len = 0;

    if (fib_len == 0) {          /* initialise the list */
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
        fib_len = 2;
    }

    idx_t bits_done = 0;   /* number of bits produced so far */
    for (idx_t k = 0; ; ++k) {

        /* ensure fib list contains all Fibonacci numbers <= k */
        while (fib_len >= 2 && fib[fib_len - 1] <= (seq_t)k) {
            if (fib_len == MAX_FIB) break;   /* safety guard */
            fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
            ++fib_len;
        }

        size_t len;              /* length of the Zeckendorf string for k */
        size_t idx_highest;      /* index of the highest Fibonacci used */

        if (k == 0) {
            len = 1;              /* representation is the single bit 0 */
            idx_highest = 0;      /* dummy, not used */
        } else {
            /* binary search for largest fib <= k */
            size_t lo = 0, hi = fib_len - 1;
            while (lo < hi) {
                size_t mid = (lo + hi + 1) >> 1;
                if (fib[mid] <= (seq_t)k)
                    lo = mid;
                else
                    hi = mid - 1;
            }
            idx_highest = lo;
            len = idx_highest + 1;  /* bits 0 … idx_highest */
        }

        if (bits_done + len > n) {
            /* the sought bit lies in this representation */
            idx_t pos = n - bits_done;   /* 0‑based within this block */

            if (k == 0)                /* the only bit is 0 */
                return (seq_t)0;

            /* build the representation bits in an array of size <= len */
            bool bits_arr[MAX_FIB];
            for (size_t i = 0; i < len; ++i) bits_arr[i] = false;

            seq_t remaining = k;
            size_t i = idx_highest;
            while (true) {
                if (remaining >= fib[i]) {
                    bits_arr[i] = true;
                    remaining -= fib[i];
                }
                if (i == 0) break;
                --i;
            }
            return (seq_t)(bits_arr[pos] ? 1 : 0);
        }

        bits_done += len;
        /* continue to next k */
    }
}
