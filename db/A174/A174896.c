/*
 * A174896  a(n) = numbers k in increasing order such that A007955(m) = k has
 *          no solution for any m, where A007955(m) = product of divisors of
 *          m.
 * https://oeis.org/A174896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A174896 – numbers that are NOT the product of all divisors of any integer.
   The algorithm sieves representable numbers up to a growing limit and finds
   the nth missing value. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ---------- internal state ---------- */
static bool *present   = NULL;        /* present[x] == true if x is representable   */
static uint32_t *missing = NULL;      /* missing[x] = #missing values ≤ x            */
static idx_t currentLimit = 0;        /* current maximum index of the arrays         */

/* ---------- memory cleanup ---------- */
static void free_memory(void)
{
    if (present)  { free(present);  present = NULL; }
    if (missing)  { free(missing);  missing = NULL; }
    currentLimit = 0;
}

/* ---------- helper: integer square root ---------- */
static inline uint64_t uint64_sqrt(uint64_t n)
{
    /* double sqrt is accurate for unsigned 64‑bit integers up to 2^53
       (much larger than the limits needed here). */
    uint64_t x = (uint64_t)sqrt((long double)n);
    while ((x + 1) * (x + 1) <= n) ++x;
    while (x * x > n) --x;
    return x;
}

/* ---------- recompute tables up to 'limit' (inclusive) ---------- */
static void recompute(idx_t limit)
{
    free_memory();                     /* discard old data */

    size_t L = (size_t)limit;
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint8_t  *tau = (uint8_t  *)calloc(L + 1, sizeof(uint8_t));
    present   = (bool *)calloc(L + 1, sizeof(bool));
    if (!spf || !tau || !present) { exit(1); }

    /* linear sieve for smallest prime factor (spf) */
    for (size_t i = 2; i <= L; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (i <= L / i) {
                for (size_t j = i * i; j <= L; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }

    /* compute divisor counts (tau) */
    tau[1] = 1;
    for (size_t i = 2; i <= L; ++i) {
        uint32_t p = spf[i];
        size_t n = i / p;
        uint8_t exp = 1;
        while (n % p == 0) {
            n /= p;
            exp++;
        }
        tau[i] = tau[n] * (exp + 1);
    }

    /* mark all representable numbers up to 'limit' */
    present[1] = true;   /* m = 1 gives product 1 */
    for (size_t m = 2; m <= L; ++m) {
        uint8_t d = tau[m];
        uint32_t exp = d / 2;           /* integer part of d/2 */
        unsigned __int128 prod = 1;

        for (uint32_t k = 0; k < exp; ++k) {
            prod *= m;
            if (prod > limit) { break; }
        }

        if (prod <= limit && (d & 1)) { /* m is a perfect square */
            uint64_t sqrt_m = uint64_sqrt((uint64_t)m);
            prod *= sqrt_m;
        }

        if (prod <= limit) {
            present[(size_t)prod] = true;
        }
    }

    /* build missing array */
    missing = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    if (!missing) exit(1);
    missing[0] = 0;
    for (size_t i = 1; i <= L; ++i) {
        missing[i] = missing[i - 1] + (present[i] ? 0u : 1u);
    }

    currentLimit = limit;
    free(spf);
    free(tau);
}

/* ---------- ensure tables large enough for 'n' missing numbers ---------- */
static void ensure_limit(idx_t n)
{
    if (currentLimit == 0) {
        recompute(100000);              /* initial guess */
    }
    while ((idx_t)missing[currentLimit] < n) {
        idx_t newLimit = currentLimit * 2;
        if (newLimit == currentLimit)    /* overflow guard */
            newLimit = currentLimit + 1;
        recompute(newLimit);
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, but safe */
    ensure_limit(n);

    /* binary search for minimal x with missing[x] >= n */
    idx_t low  = 1, high = currentLimit;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((idx_t)missing[mid] >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}
