/*
 * A173595  The smaller member of a near-repdigit emirp pair.
 * https://oeis.org/A173595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mulmod_u64, near_repdigit_u64, reverse_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes, keep those that are
   *near‑repdigit emirps*: a prime that uses exactly two decimal
   *digits, one of them occurs n−1 times (n = number of digits),
   *its decimal reverse is also prime and uses exactly two digits
   *≤ the other member, and the prime is the smaller of the pair.
   *The sequence a(n) is the sorted list of the smaller members. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod_u64.h"
#include "../utils/near_repdigit_u64.h"
#include "../utils/reverse_u64.h"

/* ---- 64‑bit modular multiplication / exponentiation ---- */

static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1ULL) res = mulmod_u64(res, base, mod);
        base = mulmod_u64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* ---- deterministic Miller–Rabin for 64‑bit integers ---- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
        23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i=0;i<sizeof(small)/sizeof(small[0]);++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0ULL) return false;
    }
    uint64_t d = n-1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }
    static const uint64_t wf[] = {
        2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL
    };
    for (size_t i=0;i<sizeof(wf)/sizeof(wf[0]);++i) {
        uint64_t a = wf[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x==1ULL || x==n-1) continue;
        bool comp = true;
        for (int r=1;r<s;r++) {
            x = mulmod_u64(x,x,n);
            if (x==n-1){ comp=false; break;}
        }
        if (comp) return false;
    }
    return true;
}

/* ---- cache variables ---- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_candidate = 2;

/* ---- capacity management ---- */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap*2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* graceful handling for offsets < 1 */

    if ((size_t)n <= cache_len)      /* already cached */
        return cache[(size_t)n-1];

    /* generate until we have n terms */
    while (cache_len < (size_t)n) {
        /* skip even numbers >2 */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0ULL) {
            ++next_candidate;
            continue;
        }

        if (is_prime_u64(next_candidate) && near_repdigit_u64(next_candidate)) {
            uint64_t rev = reverse_u64(next_candidate);
            if (rev != next_candidate &&
                near_repdigit_u64(rev) &&
                is_prime_u64(rev) &&
                next_candidate < rev) {
                ensure_capacity(cache_len+1);
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
    return cache[(size_t)n-1];
}
