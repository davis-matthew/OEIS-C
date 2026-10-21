/*
 * A056038  Largest factorial k! such that (k!)^2 divides n!.
 * https://oeis.org/A056038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   the maximum k such that (k!)^2 divides n!, returned as a
 *            (k!)*seq_t value. The function computes k using prime valuations
 *            and returns k!
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ---------- 1. Sieve primes up to n ----------
       Allocate a boolean array and generate all primes ≤ n.
       ---------------------------------------------------- */
    idx_t size = (size_t)n + 1;
    bool *is_prime = (bool *)malloc(size);
    if (!is_prime) return (seq_t)1;                 /* fallback */
    memset(is_prime, 1, size);
    if (size > 0) is_prime[0] = false;
    if (size > 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* collect primes into a dynamic array */
    size_t primes_cap = 8;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(primes); return (seq_t)1; }
                primes = tmp;
            }
            primes[primes_cnt++] = i;
        }
    }
    free(is_prime);

    /* ---------- 2. Compute v_p(n!) for all primes ----------
       Legendre’s formula: v_p(n!) = Σ floor(n / p^i).
       ------------------------------------------------------- */
    idx_t *vp_n = (idx_t *)malloc(primes_cnt * sizeof(idx_t));
    if (!vp_n) { free(primes); return (seq_t)1; }
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        idx_t sum = 0;
        idx_t div = p;
        while (div <= n) {
            sum += n / div;
            if (div > n / p) break;   /* avoid overflow */
            div *= p;
        }
        vp_n[pi] = sum;
    }

    /* ---------- 3. Binary search for largest admissible k ----------
       k works if for every prime p: 2*v_p(k!) ≤ v_p(n!).
       ------------------------------------------------------------ */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        bool ok = true;
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint64_t p = primes[pi];
            if (p > mid) break;
            /* compute v_p(mid!) */
            idx_t sum = 0;
            idx_t div = p;
            while (div <= mid) {
                sum += mid / div;
                if (div > mid / p) break;   /* prevent overflow */
                div *= p;
            }
            if ((sum << 1) > vp_n[pi]) {  /* 2*v_p(mid!) > v_p(n!) */
                ok = false;
                break;
            }
        }
        if (ok)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t k = high;   /* largest valid k */

    /* ---------- 4. Compute k! as seq_t ----------
       ------------------------------------------------ */
    seq_t result = 1;
    for (idx_t i = 2; i <= k; ++i)
        result *= (seq_t)i;

    /* ---------- 5. Clean up ----------
       ------------------------------------------------ */
    free(primes);
    free(vp_n);

    return result;
}
