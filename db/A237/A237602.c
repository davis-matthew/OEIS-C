/*
 * A237602  Largest prime with n hexadecimal digits which is a
 *          right-truncatable prime in base 16.
 * https://oeis.org/A237602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   computed by generating all right‑truncatable primes in base 16
 *            and keeping the largest for each digit length (1 – 14)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* largest right‑truncatable prime having exactly k hex digits,
   for k = 1 … 14.  biggest[0] unused. */
static seq_t biggest[15] = {0};

/* recursively extend a right‑truncatable prime by appending one
   hexadecimal digit (0 … 15).  len is the current number of
   digits. */
static void rec(seq_t val, int len) {
    if (len > 14) return;                 /* no more than 14 digits */
    if (val > biggest[len]) biggest[len] = val;
    if (len == 14) return;                /* cannot extend further */

    for (int d = 0; d < 16; ++d) {
        seq_t newval = val * (seq_t)16 + (seq_t)d;
        if (isPrimeGeneric(newval))
            rec(newval, len + 1);
    }
}

/* one‑time construction of all right‑truncatable primes up to
   14 hex digits.  Uses a static flag to avoid recomputation. */
static void init_seq(void) {
    static bool done = false;
    if (done) return;

    /* 1‑digit hexadecimal primes: 2,3,5,7,B(0xB),D(0xD). */
    const uint64_t primes1[] = {2, 3, 5, 7, 0xB, 0xD};
    size_t i, cnt = sizeof(primes1) / sizeof(primes1[0]);
    for (i = 0; i < cnt; ++i)
        rec((seq_t)primes1[i], 1);

    done = true;
}

/* external sequence function, as required. */
seq_t a(idx_t n) {
    init_seq();
    if (n == 0) return (seq_t)0;          /* offset not defined, return 0 */
    if (n <= 14) return biggest[(int)n];
    return (seq_t)0;                      /* no such prime for n > 14 */
}
