/*
 * A048126  Becomes prime or 4 after exactly 4 iterations of f(x) = sum of
 *          prime factors of x.
 * https://oeis.org/A048126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the numbers that become prime or 4 after exactly four
 *            iterations of f(x)=sum of prime factors of x (with multiplicity)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build framework. */

/* ---------- Prime list utilities ---------- */

/* Dynamic array of primes (uint64_t). */
static uint64_t *primes = NULL;
static size_t   pcnt   = 0;
static size_t   pcap   = 0;

/* Ensure the primes array has at least 'new_cap' elements. */
static void ensure_prime_cap(size_t new_cap)
{
    if (new_cap <= pcap) return;
    size_t cap = pcap ? pcap * 2 : 8;
    while (cap < new_cap) cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    primes = tmp;
    pcap   = cap;
}

/* Add a new prime to the list. */
static void add_prime(uint64_t p)
{
    ensure_prime_cap(pcnt + 1);
    primes[pcnt++] = p;
}

/* Return true if n is prime (trial division with stored primes). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Grow the primes list up to at least sqrt(limit). */
static void generate_primes_up_to(uint64_t limit)
{
    uint64_t last = pcnt ? primes[pcnt - 1] : 1;
    uint64_t cand = last + 1;
    while ((cand * cand) <= limit) {
        bool prime = true;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) add_prime(cand);
        ++cand;
    }
}

/* ---------- Sum of prime factors with multiplicity ---------- */

static seq_t sum_prime_factors(seq_t x)
{
    seq_t sum = 0;
    uint64_t val = (uint64_t)x;          /* assume value fits in 64 bits for this build   */
    if (val == 0) return 0;
    generate_primes_up_to(val);
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > val) break;
        while (val % p == 0) {
            sum += (seq_t)p;
            val /= p;
        }
    }
    if (val > 1) sum += (seq_t)val;
    return sum;
}

/* ----------  Condition test ---------- */

static bool reaches_prime_or_four(seq_t x)
{
    seq_t cur = x;
    for (int i = 0; i < 4; ++i) {
        seq_t nxt = sum_prime_factors(cur);
        if (i == 3) { /* final iteration */
            if (nxt == (seq_t)4 || is_prime_uint64((uint64_t)nxt))
                return true;
        } else { /* earlier iterations must not hit prime/4 */
            if (nxt == (seq_t)4 || is_prime_uint64((uint64_t)nxt))
                return false;
        }
        cur = nxt;
    }
    return false;
}

/* ---------- Main sequence generation ---------- */

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;
    static uint64_t next_start = 2;   /* next integer to test */

    /* Build list of primes for the initial candidate generation. */
    if (!pcnt) add_prime(2);

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        if (reaches_prime_or_four(next_start)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!new_seq) exit(1);
                seq = new_seq;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_start;
        }
        ++next_start;
    }

    return seq[(size_t)(n - 1)];
}
