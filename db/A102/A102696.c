/*
 * A102696  Number of positive even integers that can be written as the sum of
 *          2 of the first n odd primes (not necessarily distinct).
 * https://oeis.org/A102696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct even integers that can be written as
 *            the sum of two of the first n odd primes. The function increases
 *            a set of odd primes and a bitset of reachable sums
 *            incrementally, storing the result for each n in a memoization
 *            array
 * Range:     u8 n <= 56, u16 n <= 6547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------- static state -------------------------- */
static idx_t *primes = NULL;          /* list of generated odd primes          */
static idx_t primes_count = 0;        /* number of stored primes               */
static idx_t primes_capacity = 0;     /* allocated capacity of primes          */

static uint64_t *bitset = NULL;       /* bitset of reachable sums     */
static idx_t bitset_words = 0;        /* number of 64‑bit words in bitset */

static seq_t *memo = NULL;            /* memoised a(n) values           */
static idx_t memo_next = 1;           /* next index to compute          */
static idx_t memo_cap = 0;            /* allocated capacity of memo     */

static seq_t distinct_cnt = 0;        /* current number of distinct sums */
static idx_t next_candidate = 3;      /* next odd number to test */

/* Ensure primes array can hold at least min_cap elements. */
static void ensure_primes_capacity(idx_t min_cap)
{
    if (min_cap <= primes_capacity) return;
    size_t new_cap = (primes_capacity == 0) ? 16 : primes_capacity * 2;
    if (new_cap < min_cap) new_cap = min_cap;
    primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    primes_capacity = new_cap;
}

/* Ensure memo array can hold at least min_cap elements (capacity only). */
static void ensure_memo_capacity(idx_t min_cap)
{
    if (min_cap <= memo_cap) return;
    size_t new_cap = (memo_cap == 0) ? 16 : memo_cap * 2;
    if (new_cap < min_cap) new_cap = min_cap;
    memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    memo_cap = new_cap;
}

/* Ensure bitset can hold bits up to 'bits_needed' inclusive. */
static void ensure_bitset_capacity(idx_t bits_needed)
{
    idx_t words_needed = (bits_needed + 63) / 64;   /* number of u64 words */
    if (words_needed <= bitset_words) return;

    uint64_t *newbits = (uint64_t *)realloc(bitset, words_needed * sizeof(uint64_t));
    if (!bitset) {
        /* first allocation: zero whole block */
        memset(newbits, 0, words_needed * sizeof(uint64_t));
    } else {
        /* zero only new part */
        memset(newbits + bitset_words, 0,
               (words_needed - bitset_words) * sizeof(uint64_t));
    }
    bitset = newbits;
    bitset_words = words_needed;
}

/* Set a bit in the bitset; if it was not set before, increment counter. */
static inline void set_bit(idx_t pos)
{
    idx_t word = pos >> 6;
    uint64_t mask = 1ULL << (pos & 63);
    if (!(bitset[word] & mask)) {
        bitset[word] |= mask;
        distinct_cnt++;
    }
}

/* Generate the next odd prime and store it in primes[]. */
static idx_t generate_next_prime(void)
{
    idx_t cand = next_candidate;
    while (true) {
        bool prime = true;
        for (idx_t i = 0; i < primes_count; ++i) {
            idx_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) break;          /* cand is prime */
        cand += 2;                  /* next odd candidate */
    }
    next_candidate = cand + 2;      /* update for future calls */

    /* store prime */
    ensure_primes_capacity(primes_count + 1);
    primes[primes_count++] = cand;
    return cand;
}

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* unused, but defined for safety */

    /* Compute further values up to n if needed */
    if (n >= memo_next) {
        /* adjust memo array capacity */
        ensure_memo_capacity(n + 1);

        for (idx_t k = memo_next; k <= n; ++k) {
            /* generate next prime for index k */
            idx_t p = generate_next_prime();

            /* Ensure bitset can hold sums up to 2*p */
            ensure_bitset_capacity(2 * p + 1);

            /* add sums of p with all primes (including itself) */
            for (idx_t j = 0; j < primes_count; ++j) {
                set_bit(p + primes[j]);
            }

            /* store value of a(k) */
            memo[k] = distinct_cnt;
        }

        memo_next = n + 1;  /* next index to compute */
    }

    return memo[n];
}
