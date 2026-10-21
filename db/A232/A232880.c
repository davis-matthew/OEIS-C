/*
 * A232880  Twin primes with digital root 2 or 4.
 * https://oeis.org/A232880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232880: twin primes whose digital roots are 2 and 4.
   Computed by searching twin primes and selecting those whose
   digital roots are 2 and 4. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *list = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static uint64_t next_check = 5;

    while (len < n) {
        uint64_t p = next_prime(next_check);
        next_check = p + 1;

        if (is_prime(p + 2)) {
            uint64_t modp = p % 9;
            int dr1 = (int)(modp == 0 ? 9 : modp);
            uint64_t modp2 = (modp + 2) % 9;
            int dr2 = (int)(modp2 == 0 ? 9 : modp2);

            if ((dr1 == 2 && dr2 == 4) || (dr1 == 4 && dr2 == 2)) {
                if (len + 2 > cap) {
                    size_t new_cap = (cap == 0) ? 8 : cap * 2;
                    while (new_cap < len + 2) new_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    list = tmp;
                    cap = new_cap;
                }
                list[len]     = (seq_t)p;
                list[len + 1] = (seq_t)(p + 2);
                len += 2;
            }
        }
    }
    return list[n - 1];
}
