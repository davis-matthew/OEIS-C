/*
 * A165451  Sum of factorial of digits is prime.
 * https://oeis.org/A165451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 5, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_digits, isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165451: Sum of factorial of digits is prime.
   Return a(n) = the n‑th positive integer whose digits whose factorials
   sum to a prime number (1‑based indexing). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_digits.h"     /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t n); */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *p, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1 – safe return */

    /* static storage for generated sequence */
    static seq_t *values = NULL;
    static size_t val_cnt = 0, val_cap = 0;
    static seq_t last_checked = 1;        /* candidate integer to test */

    /* pre‑computed factorials of 0…9 (fits in any seq_t) */
    static const seq_t fact[10] = {
        1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
        120ULL, 720ULL, 5040ULL,
        40320ULL, 362880ULL
    };

    /* generate terms until we have at least n of them */
    while (val_cnt < n) {
        last_checked++;                    /* next integer to test */

        /* digits of last_checked */
        uint8_t digs[64];                  /* enough for < 10^64 */
        size_t dlen;
        get_digits(last_checked, digs, &dlen);

        /* sum of factorial of its digits */
        seq_t sum = 0;
        for (size_t i = 0; i < dlen; ++i)
            sum += fact[digs[i]];

        /* if the sum is prime, record the candidate */
        if (isPrimeGeneric(sum)) {
            if (val_cnt == val_cap) {
                val_cap = val_cap ? val_cap * 2 : 128;
                seq_t *tmp = seq_realloc(values, val_cnt, val_cap);
                if (!tmp) abort();          /* memory failure */
                values = tmp;
            }
            values[val_cnt++] = last_checked;
        }
    }

    return values[n - 1];                    /* 1‑based index */
}
