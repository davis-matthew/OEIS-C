/*
 * A078279  a(n) is the number from which if odd-positioned digits are deleted
 *          then one gets the concatenation of first n composite numbers,
 *          while if even-positioned digits are deleted then one gets reverse
 *          concatenation of first n composite numbers. Position of least
 *          significant digit is considered to be 1.
 * https://oeis.org/A078279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   append_forward, digit_count_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A078279:
   a(n) is the number from which deleting the odd‑positioned
   digits (least significant digit position 1) yields the
   concatenation of the first n composite numbers,
   and deleting the even‑positioned digits yields the
   reverse concatenation of the first n composite numbers. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_forward.h"
#include "../utils/digit_count_uint32.h"

/* ----------------------------------------------------------------- */
/* global state to cache composites                                       */
static uint32_t *composites      = NULL;   /* array of first composites   */
static size_t   composites_cnt   = 0;      /* how many are stored        */
static size_t   composites_cap   = 0;      /* allocated capacity         */
static uint32_t last_limit       = 0;      /* last integer that was
                                            processed for composites */
static void ensure_composites(size_t need)
{
    if (composites_cnt >= need) return;      /* already enough        */

    /* start with a rough upper bound 2*need, at least 4 */
    size_t limit = (need * 2 > 4 ? need * 2 : 4);

    while (composites_cnt < need) {
        /* sieve up to new limit */
        bool *is_prime = (bool*)malloc(limit + 1);
        if (!is_prime) abort();
        for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;

        /* enlarge composites array if needed */
        if (composites_cap < composites_cnt + (limit - last_limit)) {
            size_t new_cap = composites_cap ? composites_cap * 2 : 1024;
            while (new_cap < composites_cnt + (limit - last_limit))
                new_cap *= 2;
            composites = (uint32_t*)realloc(composites, new_cap * sizeof(uint32_t));
            composites_cap = new_cap;
        }

        for (size_t i = last_limit + 1; i <= limit; ++i) {
            if (i >= 4 && !is_prime[i]) {
                composites[composites_cnt++] = (uint32_t)i;
            }
        }

        free(is_prime);
        last_limit = limit;
        limit = limit * 2;          /* prepare bigger limit if needed */
    }
}

/* ----------------------------------------------------------------- */
/* sequence function                                                  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, return 0 */

    size_t need = (size_t)n;
    ensure_composites(need);

    /* total number of digits in the forward concatenation */
    size_t lenC = 0;
    for (size_t i = 0; i < need; ++i)
        lenC += digit_count_uint32(composites[i]);

    /* allocate buffers for forward and reverse strings */
    char *C = (char*)malloc(lenC);
    char *D = (char*)malloc(lenC);
    if (!C || !D) abort();

    size_t posC = 0, posD = 0;
    for (size_t i = 0; i < need; ++i)
        append_forward(composites[i], C, &posC);

    for (size_t i = need; i > 0; --i)
        append_forward(composites[i-1], D, &posD);

    /* build the result by interleaving digits */
    seq_t result = 0;
    for (size_t i = 0; i < lenC; ++i) {
        result = result * 10 + (seq_t)(C[i] - '0');
        result = result * 10 + (seq_t)(D[i] - '0');
    }

    free(C);
    free(D);
    return result;
}
