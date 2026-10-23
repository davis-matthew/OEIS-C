/*
 * A129336  Digital clock semiprimes.
 * https://oeis.org/A129336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) is the nth semiprime number that can appear as a 24‑hour *
 *            digital clock time HH:MM (0 ≤ HH ≤ 23, 0 ≤ MM ≤ 59). The set of
 *            * possible numbers is {100·h + m | h = 0..23, m = 0..59}
 * Range:     u8 n <= 60, u16 n <= 414, u32 n <= 414, u64 n <= 414,
 *            u128 n <= 414, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper to test semiprime -------------------------------- */

static bool is_semiprime(uint16_t v)
{
    if (v < 4) return false;          /* smaller than 2·2 */

    /* primes up to √2359 < 50 */
    static const uint16_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19,
        23, 29, 31, 37, 41, 43, 47
    };

    uint16_t temp = v;
    int count = 0;

    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        uint16_t p = primes[i];
        if (p * p > temp) break;
        while (temp % p == 0) {
            temp /= p;
            ++count;
            if (count > 2) return false;
        }
        if (temp == 1) break;
    }
    if (temp > 1) ++count;            /* prime residue */

    return count == 2;
}

/* ---------- Cache of all sequence terms ----------------------------- */

static seq_t *cache = NULL;         /* array of terms */
static size_t cache_len = 0;        /* number of terms stored */
static bool initialized = false;    /* single‑time init flag */

static void init_cache(void)
{
    if (initialized) return;

    /* maximum possible number of terms is 414, so 500 is a safe cap. */
    cache = (seq_t*)malloc(500 * sizeof(seq_t));
    if (!cache) return;             /* allocation failure: leave empty */

    cache_len = 0;
    for (uint16_t h = 0; h <= 23; ++h) {
        for (uint16_t m = 0; m <= 59; ++m) {
            uint16_t v = h * 100 + m;   /* HHMM as a simple integer */
            if (is_semiprime(v)) {
                cache[cache_len++] = (seq_t)v;
            }
        }
    }
    initialized = true;
}

/* ---------- The required OEIS function ----------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but return 0 */

    init_cache();                    /* build cache once */

    if (n - 1U >= cache_len) return 0;   /* out of range – not defined */

    return cache[n - 1U];
}
