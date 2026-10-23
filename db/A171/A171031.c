/*
 * A171031  In the sequence of prime numbers, replace all the '6' digits with
 *          '3' and vice versa.
 * https://oeis.org/A171031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   swap_digits_3_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A171031
 *  Formula:  a(n) = digitSwap_3_6(p_n)
 *  where p_n is the n‑th prime (a(1)=2, a(2)=3→swap→6, etc.).
 *  digitSwap_3_6 exchanges decimal digit '3' with '6'.
 *
 *  The implementation generates primes incrementally and
 *  applies the digit swap lazily.  It works for any
 *  integer type defined as seq_t and idx_t by the build.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/swap_digits_3_6.h"

/* ---------- Prime generation (static table) ---------- */

static uint32_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint32_t next_candidate = 3;

/* Ensure that at least 'need' primes have been generated. */
static void
ensure_primes_upto(idx_t need)
{
    /* Initial allocation for primes. */
    if (prime_tbl == NULL) {
        prime_cap = 1024;
        prime_tbl = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_tbl) exit(1);
        prime_tbl[0] = 2;          /* first prime */
        prime_cnt = 1;
    }

    while (prime_cnt < (size_t)need) {
        uint32_t cand = next_candidate;
        next_candidate += 2;          /* skip evens */

        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_tbl[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cnt == prime_cap) {          /* grow table */
                prime_cap = prime_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_tbl, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_tbl = tmp;
            }
            prime_tbl[prime_cnt++] = cand;
        }
    }
}

/* ---------- Swap digits 3↔6 for a 32‑bit integer ---------- */

/* ---------- Public interface a(idx_t n) ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined; offset starts at 1 */
    ensure_primes_upto(n);

    /* primes are zero‑based: primes[0] = 2 = p_1 */
    uint32_t p = prime_tbl[n - 1];
    return swap_digits_3_6(p);
}
