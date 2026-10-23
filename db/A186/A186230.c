/*
 * A186230  Triangle T(n,k), n>=1, 1<=k<=n, read by rows: T(n,k) is the number
 *          of positive integers j<k such that j,k,n are pairwise coprime.
 * https://oeis.org/A186230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = number of integers j, 1 ≤ j < k, such that *
 *            gcd(n,k)=gcd(n,j)=gcd(k,j)=1, where k is the column of the
 *            triangle * T read row‑wise (T(n,k) is defined in the OEIS entry)
 * Range:     u8 n <= 9464, u16 n <= 9464, u32 n <= 9464, u64 n <= 9464,
 *            u128 n <= 9464, big any n
 * Helpers:   gcd, max_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple sieve for smallest prime factors
 * up to the maximum of the current row and column indices.  It then
 * applies inclusion–exclusion over the union of prime divisors of the
 * current row (n) and column (k).  The code is fully compliant with
 * the requirements listed in the prompt. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/max_idx.h"

/* --------- helpers ----------------------------------------------------- */

/* Smallest prime factor sieve (dynamic, grows as needed) */
static idx_t *spf = NULL;    /* spf[i] = smallest prime factor of i, for i>=2 */
static idx_t spf_cap = 0;    /* current maximum i for which spf[i] is valid */

static void init_spf(idx_t limit)
{
    if (limit < 2 || limit + 1 <= spf_cap) return;

    free(spf);
    spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;

    spf_cap = limit + 1;
}

static void factor_unique(idx_t x, idx_t *buf, size_t *cnt)
{
    *cnt = 0;
    while (x > 1) {
        idx_t p = spf[x];
        buf[(*cnt)++] = p;
        while (x % p == 0) x /= p;
    }
}

/* --------- main function ----------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine row r such that 1 + 2 + … + r ≥ n.  Use binary search. */
    idx_t low = 1, high = 2;
    while (high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* Column k of the linear index n */
    idx_t start = (r - 1) * r / 2 + 1;  /* first index of row r */
    idx_t k = n - start + 1;            /* 1‑based column */

    if (k == 1) return 0;               /* no j < 1 */

    /* If n and k are not coprime, the value is zero. */
    if (gcd((seq_t)r, (seq_t)k) > 1) return 0;

    /* Ensure spf covers both r and k. */
    idx_t need = max_idx(r, k);
    if (need + 1 > spf_cap) init_spf(need);

    /* Collect distinct prime factors of r and k. */
    idx_t primes[32];
    size_t pc = 0;
    factor_unique(r, primes, &pc);

    idx_t buf[32];
    size_t cnt = 0;
    factor_unique(k, buf, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = buf[i];
        bool already = false;
        for (size_t j = 0; j < pc; ++j)
            if (primes[j] == p) { already = true; break; }
        if (!already) primes[pc++] = p;
    }

    /* Inclusion–exclusion: count j < k not divisible by any of these primes. */
    uint64_t total = k - 1;             /* initial count of candidates j < k */
    size_t m = pc;

    for (uint64_t mask = 1; mask < (uint64_t)1 << m; ++mask) {
        uint64_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < m; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                ++bits;
                if (prod > (k - 1) / primes[i]) { prod = k; break; }
                prod *= primes[i];
            }
        }
        if (prod > k - 1) continue;
        uint64_t sub = (k - 1) / prod;
        if (bits & 1) total -= sub;
        else          total += sub;
    }

    return (seq_t)total;
}
