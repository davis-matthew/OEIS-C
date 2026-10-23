/*
 * A123722  Values of n for which A000224(n)=Ceiling[(n+1)/3]; A000224(n) is
 *          the number of squares mod n.
 * https://oeis.org/A123722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m such that A000224(m) = ceil((m+1)/3)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   count_odd_power, count_pow2_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of quadratic residues modulo a prime power
   is computed exactly; counted for both odd primes and 2.   */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/count_odd_power.h"
#include "../utils/count_pow2_pow.h"

/* ------------ small prime sieve --------------------------------- */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static uint64_t prime_candidate = 3;  /* next odd to test */

static void ensure_primes_upto(uint64_t limit)
{
    if (primes_len == 0) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_len = 1;
        prime_candidate = 3;
    }
    while (prime_candidate <= limit) {
        bool is_prime = true;
        uint64_t cand = prime_candidate;
        uint64_t root = (uint64_t)sqrt((double)cand);
        for (size_t i = 0; i < primes_len && primes[i] <= root; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_len >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
        }
        prime_candidate += 2;
    }
}

/* -------------- count quadratic residues modulo n ---------------- */
static inline seq_t qresidue_count(uint64_t n)
{
    uint64_t temp = n;
    uint64_t k = 0;
    while ((temp & 1ULL) == 0) { ++k; temp >>= 1; }

    seq_t count = count_pow2_pow(k);

    if (temp == 1) return count;   /* no odd part */

    /* factor the odd part */
    ensure_primes_upto((uint64_t)(sqrt((double)temp) + 1));
    uint64_t t = temp;
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= t; ++i) {
        uint64_t p = primes[i];
        if (t % p == 0) {
            uint64_t a = 0;
            while (t % p == 0) { t /= p; ++a; }
            count *= count_odd_power(p, a);
        }
    }
    if (t > 1) {          /* remaining prime */
        count *= count_odd_power(t, 1);
    }
    return count;
}

/* -------------- check the defining condition --------------------- */
static inline bool satisfies(uint64_t n)
{
    seq_t sq = qresidue_count(n);
    seq_t target = ((seq_t)n + 3) / 3;          /* ceil((n+1)/3) */
    return sq == target;
}

/* ------------------------------------------- sequence storage --- */
static seq_t *seq     = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* --------------------------- public function ---------------- ----- */
seq_t a(idx_t n)
{
    static uint64_t checked = 0;     /* largest m already examined */

    /* extend cache until we have n terms */
    while (seq_len < (size_t)n) {
        ++checked;
        if (satisfies(checked)) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)checked;
        }
    }
    return seq[n - 1];
}
