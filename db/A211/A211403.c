/*
 * A211403  The smallest n-digit number where last k digits are divisible by
 *          k-th prime for k = 1..n.
 * https://oeis.org/A211403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the smallest n‑digit number N such that for * each k =
 *            1..n the last k digits of N are divisible by the k‑th prime. *
 *            This is computed by building N from the least significant digit
 *            * upward, keeping all possible partial numbers that satisfy the
 *            * divisibility conditions. The algorithm wo...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static inline uint64_t modpow_uint(uint64_t base, uint64_t exp, uint32_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* cache results for 1..9 (n > 9 never appears in OEIS) */
    static seq_t cache[10];
    static bool has[10] = {false};
    if (n == 0) return 0;              /* offset is 1 – 0 is unused */
    if (n > 9) return 0;               /* no further terms known */

    if (has[n]) return cache[n];
    has[n] = true;

    /* Special case n = 1 */
    if (n == 1) {
        cache[1] = 0;                  /* smallest 1‑digit number divisible by 2 */
        return cache[1];
    }

    /* Candidate arrays */
    seq_t cur[200], nxt[200];
    size_t cur_len = 0, nxt_len = 0;

    /* Initialize with all 1‑digit numbers divisible by the first prime (2) */
    for (uint8_t d = 0; d <= 8; d += 2) cur[cur_len++] = (seq_t)d;

    /* Build numbers digit by digit (least significant → most significant) */
    for (idx_t k = 2; k <= n; ++k) {
        uint32_t prime = get_prime(k);
        seq_t pow10_seq = 1;          /* 10^(k-1) as seq_t */
        uint64_t pow10_mod = 1;       /* 10^(k-1) mod prime */

        for (idx_t i = 1; i < k; ++i) {
            pow10_seq *= (seq_t)10;
            pow10_mod = (pow10_mod * 10) % prime;
        }

        /* modular inverse of pow10_mod modulo prime, if it exists */
        bool has_inv = (pow10_mod % prime != 0);
        uint64_t inv = 0;
        if (has_inv) {
            /* pow10_mod and prime are coprime (prime ∤ 10) */
            inv = modpow_uint(pow10_mod, prime - 2, prime);
        }

        nxt_len = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            seq_t S = cur[i];
            uint64_t S_mod = (uint64_t)(S % (seq_t)prime);
            uint64_t base = (prime - S_mod) % prime;   /* -S_mod mod prime */

            /* generate all digits d in [0,9] satisfying the congruence */
            if (!has_inv) {
                /* pow10_mod == 0 mod prime → need S_mod == 0, and any d works */
                if (S_mod % prime != 0) continue;
                for (uint8_t d = 0; d <= 9; ++d) {
                    if (k == n && d == 0) continue;    /* leading zero forbidden */
                    nxt[nxt_len++] = d * pow10_seq + S;
                }
            } else {
                uint64_t d0 = (base * inv) % prime;   /* smallest solution mod prime */
                for (uint64_t t = 0;; ++t) {
                    uint64_t d = d0 + t * prime;
                    if (d > 9) break;
                    if (k == n && d == 0) continue;    /* leading zero forbidden */
                    nxt[nxt_len++] = (uint64_t)d * pow10_seq + S;
                }
            }
        }

        /* move next candidates to current */
        for (size_t i = 0; i < nxt_len; ++i) cur[i] = nxt[i];
        cur_len = nxt_len;
        if (cur_len == 0) break;                 /* no solutions */
    }

    /* no valid n‑digit number */
    if (cur_len == 0) {
        cache[n] = 0;
        return 0;
    }

    /* find minimal among remaining candidates */
    seq_t best = cur[0];
    for (size_t i = 1; i < cur_len; ++i)
        if (cur[i] < best) best = cur[i];

    cache[n] = best;
    return best;
}
