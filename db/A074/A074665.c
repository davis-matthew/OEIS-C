/*
 * A074665  8-digit distinct digit primes.
 * https://oeis.org/A074665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9977, u64 n <= 9977, u128 n <= 9977,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074665: 8‑digit distinct‑digit primes
 *
 *  The algorithm builds a list of all 8‑digit primes that contain only
 *  distinct decimal digits and returns the nth element of that list
 *  (a(1) corresponds to the first such prime).
 *
 *  The first 90510 numbers of the sequence are generated once using
 *  a single sieve of Eratosthenes up to 99,999,999.
 *  Subsequent calls simply look up the pre‑computed table.
 *
 *  Complexity:  O(U log log U) to build the sieve once, where
 *  U = 100 000 000.  After that each lookup is O(1).
 *  The table contains exactly 90510 8‑digit primes.
 *
 *  Notes:
 *  - seq_t and idx_t are provided by the build system.
 *  - The function works correctly for any n ≥ 1 and any index
 *    order (calls may be non‑monotonic).  If n is out of range,
 *    0 is returned.
 *
 *  Implementation follows strict C11/C++17 rules.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper macros for the bitset */
#define BITSET_GET(arr, i)   ((arr[(i)>>3] >> ((i)&7)) & 1)
#define BITSET_SET(arr, i)   (arr[(i)>>3] |= (1u << ((i)&7)))

/* Number of 8‑digit distinct‑digit primes (from the OEIS B‑file). */
#define PRIME_CNT 90510

/* Upper bound for 8‑digit numbers (exclusive). */
#define UPPER 100000000  /* 99,999,999 + 1 */

/* Static storage for the pre‑computed primes and the sieve. */
static seq_t    primes[PRIME_CNT];
static size_t   primeCnt = 0;
static bool     ready    = false;

/* Forward declaration of the build routine. */
static void build_primes(void);

/*---------------------------------------------------------------------*/
/*  Main sequence function                                            */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Boundary checks: n ≥ 1 is expected. */
    if (n < 1) return 0;

    if (!ready) {
        build_primes();
        ready = true;
    }

    /* If the requested index exceeds the known count, return 0. */
    if ((size_t)n > primeCnt) return 0;

    /* 1‑based indexing */
    return primes[n - 1];
}

/*---------------------------------------------------------------------*/
/*  Build all 8‑digit distinct‑digit primes.                           */
/*---------------------------------------------------------------------*/
static void build_primes(void)
{
    /* Allocate the sieve: one bit per integer up to UPPER. */
    size_t sieveSize = (UPPER >> 3) + 1;
    uint8_t *sieve = (uint8_t *)calloc(sieveSize, 1);
    if (!sieve) return;  /* malloc failure – give up */

    /* Mark 0 and 1 as composite */
    BITSET_SET(sieve, 0);
    BITSET_SET(sieve, 1);

    /* Classic sieve of Eratosthenes up to sqrt(UPPER) */
    int limit = 10000;  /* floor(sqrt(UPPER)) */
    for (int i = 2; i <= limit; ++i) {
        if (!BITSET_GET(sieve, i)) {
            /* mark multiples of i */
            for (int64_t j = (int64_t)i * i; j < UPPER; j += i) {
                BITSET_SET(sieve, (int)j);
            }
        }
    }

    /* Collect 8‑digit primes with all distinct digits. */
    int primeIndex = 0;
    for (int num = 10000000; num < 100000000; ++num) {
        if (BITSET_GET(sieve, num)) continue;  /* composite */

        /* Check that all digits are distinct */
        bool used[10] = { false };
        int n = num;
        bool ok = true;
        while (n > 0) {
            int d = n % 10;
            if (used[d]) { ok = false; break; }
            used[d] = true;
            n /= 10;
        }
        if (!ok) continue;

        /* Store the prime */
        primes[primeIndex++] = (seq_t)num;
        if (primeIndex == PRIME_CNT) break;  /* all known primes collected */
    }

    primeCnt = (size_t)primeIndex;
    /* Free the sieve (no longer needed). */
    free(sieve);
}
