/*
 * A101784  Primes which remain prime after omitting maximal digit.
 * https://oeis.org/A101784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = prime p for which deleting the leftmost maximal digit of
 *            its decimal representation yields another prime
 * Range:     u8 n <= 26, u16 n <= 1973, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static bool ready = false;
    static seq_t *list = NULL;
    static idx_t list_size = 0;

    if (!ready) {
        /* Build sieve up to a safe limit (20 000 000). */
        const idx_t LIMIT = 20000000ULL;
        uint8_t *isPrime = (uint8_t*)malloc((size_t)(LIMIT + 1));
        if (!isPrime) return 0;                     /* out of memory */

        memset(isPrime, 1, (size_t)(LIMIT + 1));
        isPrime[0] = isPrime[1] = 0;
        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (isPrime[i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    isPrime[j] = 0;
            }
        }

        /* Allocate list of qualifying primes. */
        size_t cap = 1000;
        list = (seq_t*)malloc(cap * sizeof(seq_t));
        list_size = 0;

        /* Scan all primes, test property, and store valid ones. */
        for (idx_t p = 2; p <= LIMIT; ++p) {
            if (!isPrime[p]) continue;

            uint64_t prime = p;

            /* Find maximal decimal digit. */
            uint8_t maxdigit = 0;
            uint64_t tmp = prime;
            while (tmp > 0) {
                uint8_t d = (uint8_t)(tmp % 10);
                if (d > maxdigit) maxdigit = d;
                tmp /= 10;
            }

            /* Determine highest power of 10 in p. */
            uint64_t pow10 = 1;
            tmp = prime;
            while (tmp / 10 > 0) {
                tmp /= 10;
                pow10 *= 10;
            }

            /* Build the number after removing the leftmost maximal digit. */
            bool skipped = false;
            uint64_t newp = 0;
            while (pow10 > 0) {
                uint8_t d = (uint8_t)((prime / pow10) % 10);
                if (!skipped && d == maxdigit)
                    skipped = true;
                else
                    newp = newp * 10 + d;
                pow10 /= 10;
            }

            /* Check that the resulting number is a prime. */
            if (newp >= 2 && newp <= LIMIT && isPrime[newp]) {
                if (list_size == cap) {
                    cap <<= 1;
                    list = (seq_t*)realloc(list, cap * sizeof(seq_t));
                }
                list[list_size++] = (seq_t)prime;
            }
        }

        free(isPrime);
        ready = true;
    }

    /* Return the n-th term (1‑based index). */
    if (n == 0 || n > list_size) return 0;
    return list[n - 1];
}
