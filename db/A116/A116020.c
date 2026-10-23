/*
 * A116020  Numbers n such that sigma(n) - phi(n) is a repdigit greater than
 *          2.
 * https://oeis.org/A116020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   calc_phi, ensure_capacity, get_digits, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for n such that sigma(n) - phi(n) is a repdigit greater than 2 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"
#include "../utils/get_digits.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static uint64_t next = 1;                   /* next candidate to test */

    while (sz < n) {
        idx_t cand = next++;
        seq_t sig  = sigma_of(cand);
        seq_t phi  = calc_phi((seq_t)cand);
        seq_t diff = sig - phi;
        /* diff must be > 2 and a repdigit */
        if (diff > (seq_t)2) {
            size_t len;
            uint8_t buf[128];
            get_digits(diff, buf, &len);
            if (len > 0) {
                bool rep = true;
                uint8_t d = buf[0];
                for (size_t i = 1; i < len; ++i) {
                    if (buf[i] != d) { rep = false; break; }
                }
                if (rep) {
                    ensure_capacity(sz + 1, &cap, (void **)&cache, sizeof(seq_t));
                    cache[sz++] = (seq_t)cand;
                }
            }
        }
    }
    return cache[n - 1];
}
