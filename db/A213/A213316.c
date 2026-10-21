/*
 * A213316  Numbers with exactly 9 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6904, u32 n <= 9002, u64 n <= 9002,
 *            u128 n <= 9002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213316 – numbers with exactly nine nonprime decimal substrings
 *
 *  The implementation searches for the terms sequentially.
 *  It pre‑computes primality up to 10⁷ using a sieve and scans
 *  successive integers beginning at 1002 until the requested index
 *  is reached.  The sequence is finite; indices beyond the last
 *  term (9973331) return 0.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Sieve of Eratosthenes for primes up to 10⁷ (exclusive)              */
/* ------------------------------------------------------------------ */
static const idx_t MAX_PRIME_LIMIT = 10000000;      /* numbers < 10⁷ are needed   */
static bool *prime_flag = NULL;                     /* true  -> prime, false -> composite */
static void init_prime_sieve(void)
{
    if (prime_flag != NULL) return;                 /* already initialized */

    prime_flag = (bool *)malloc((MAX_PRIME_LIMIT + 1) * sizeof(bool));
    if (prime_flag == NULL) exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= MAX_PRIME_LIMIT; ++i) prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;

    for (idx_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                prime_flag[j] = false;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Helpers to extract decimal digits (most‑significant first)          */
/* ------------------------------------------------------------------ */
static void get_digits(idx_t x, uint8_t *dig, idx_t *len)
{
    uint8_t tmp[10];
    idx_t l = 0;
    do {
        tmp[l++] = x % 10;
        x /= 10;
    } while (x);
    /* reverse into dig */
    for (idx_t i = 0; i < l; ++i) dig[i] = tmp[l - 1 - i];
    *len = l;
}

/* ------------------------------------------------------------------ */
/*  Storage for discovered terms                                        */
/* ------------------------------------------------------------------ */
static uint64_t *terms_u64   = NULL;   /* array holding term values as 64‑bit */
static size_t   terms_cnt    = 0;      /* number of terms stored */
static size_t   terms_cap    = 0;      /* capacity of the array         */
static idx_t    last_checked = 1001;   /* next candidate will start at 1002 */

/* ------------------------------------------------------------------ */
/*  Count nonprime substrings of a number, early‑exiting after >9      */
/* ------------------------------------------------------------------ */
static inline bool has_ninnonprimes(idx_t num)
{
    uint8_t dig[10];
    idx_t len;
    get_digits(num, dig, &len);

    idx_t count = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned val = 0;
        bool leading_zero = (dig[i] == 0);
        for (idx_t j = i; j < len; ++j) {
            val = val * 10 + dig[j];
            bool nonprime = leading_zero || !prime_flag[val];
            if (nonprime) {
                if (++count > 9) return false;   /* too many nonprime substrings */
            }
        }
    }
    return count == 9;
}

/* ------------------------------------------------------------------ */
/*  Main function required by the exporter                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* sequence is 1‑based */

    /* initialise sieve once */
    if (prime_flag == NULL) init_prime_sieve();

    /* extend cache until it contains at least n terms */
    while (terms_cnt < (size_t)n) {
        ++last_checked;
        if (last_checked > MAX_PRIME_LIMIT) break;   /* safety guard */

        if (has_ninnonprimes(last_checked)) {
            /* ensure capacity */
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                uint64_t *p = (uint64_t *)realloc(terms_u64, new_cap * sizeof(uint64_t));
                if (p == NULL) exit(EXIT_FAILURE);
                terms_u64 = p;
                terms_cap = new_cap;
            }
            terms_u64[terms_cnt++] = (uint64_t)last_checked;
        }
    }

    if (n <= terms_cnt) return (seq_t)terms_u64[n - 1];
    /* index beyond the known finite sequence */
    return 0;
}
