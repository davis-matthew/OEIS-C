/*
 * A097221  Numbers n such that for some k and a_1,a_2,...,a_k the
 *          concatenation of the a_i is equal to n and their product is equal
 *          to pi(n).
 * https://oeis.org/A097221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097221: Numbers n such that there exist parts a₁,…,a_k with
   a₁a₂…a_k = n and a₁·a₂·…·a_k = π(n), where π(n) is the number of primes ≤ n.
   The function a(idx_t n) returns the n‑th term (1‑based index).  */

/* ----  CONSTANTS ------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Maximum sieve size.  5×10⁷ bits ≈ 6.3 MB.  This covers more than enough
   values to produce the first ten thousand terms. */
#define SIEVE_LIMIT 50000000U

/* ----  GLOBALS ----------------------------------------------------------- */

static bool sieve_inited = false;
static uint32_t *sieve_bits = NULL;     /* bitset: sieve_bits[i] → prime i? */
static idx_t pi_count = 0;              /* prime count <= current n                */
static idx_t current_n = 1;             /* next n to process                       */
static seq_t *terms = NULL;             /* computed sequence values                */
static size_t terms_cap = 0;             /* capacity of terms[]                    */
static size_t terms_len = 0;             /* number of terms stored                 */

/* ----  BITSET ACCESSORS ------------------------------------------------- */

static inline void set_bit(idx_t n)
{
    sieve_bits[n >> 5] |= (1U << (n & 31));
}
static inline void clear_bit(idx_t n)
{
    sieve_bits[n >> 5] &= ~(1U << (n & 31));
}
static inline bool test_bit(idx_t n)
{
    return (sieve_bits[n >> 5] >> (n & 31)) & 1U;
}

/* ----  SAWPIEUL INIT ----------------------------------------------------- */

static void init_sieve(void)
{
    if (sieve_inited) return;
    size_t bits_len = (SIEVE_LIMIT >> 5) + 1;
    sieve_bits = (uint32_t *)malloc(bits_len * sizeof(uint32_t));
    if (!sieve_bits) exit(1);         /* bail out on OOM */
    memset(sieve_bits, 0, bits_len * sizeof(uint32_t));

    /* Initially mark every number ≥2 as prime */
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) set_bit(i);

    /* Classic Eratosthenes */
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (test_bit(i)) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                clear_bit(j);
        }
    }
    sieve_inited = true;
}

/* ----  ADD TERM TO SEQUENCE --------------------------------------------- */

static inline void add_term(idx_t n)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)n;
}

/* ----  MAIN FUNCTION ------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Ensure sieve and static structures are in place. */
    init_sieve();

    /* Grow the sequence until we have at least n terms. */
    while (terms_len < n) {
        if (current_n > SIEVE_LIMIT) {
            /* In the unlikely event the limit is exceeded, just exit. */
            /* The build typically guarantees this never happens. */
            exit(1);
        }

        /* Update prime counter for current_n. */
        if (current_n >= 2 && test_bit(current_n))
            pi_count++;

        /* Process candidates only for n ≥ 16, because smaller numbers cannot
           satisfy the condition (product of parts is at least 1). */
        if (current_n >= 16) {
            /* Build digit array (most significant first). */
            int digits[20];   /* enough for up to 20 digits of 64‑bit numbers */
            int len = 0;
            idx_t temp = current_n;
            while (temp > 0) {
                digits[len++] = temp % 10;
                temp /= 10;
            }
            /* Reverse to MSB first */
            for (int i = 0; i < len / 2; ++i) {
                int t = digits[i];
                digits[i] = digits[len - 1 - i];
                digits[len - 1 - i] = t;
            }

            /* At least one split required: masks 1 .. (1 << (len-1))-1 */
            if (len > 1) {
                int max_mask = 1 << (len - 1);
                for (int mask = 1; mask < max_mask; ++mask) {
                    unsigned __int128 prod = 1;
                    unsigned __int128 cur  = 0;
                    bool ok = true;
                    for (int i = 0; i < len; ++i) {
                        cur = cur * 10 + digits[i];
                        /* split after position i? */
                        if (mask & (1 << i)) {
                            if (cur == 0) {      /* leading zero or zero part */
                                ok = false;
                                break;
                            }
                            prod *= cur;
                            if (prod > (unsigned __int128)pi_count) {
                                ok = false;
                                break;
                            }
                            cur = 0;
                        }
                    }
                    if (!ok) continue;
                    if (cur == 0) continue;      /* last part zero */
                    prod *= cur;
                    if (prod == (unsigned __int128)pi_count) {
                        add_term(current_n);
                        break; /* nothing else to check for this n */
                    }
                }
            }
        }

        ++current_n;
    }

    return terms[n - 1];
}
