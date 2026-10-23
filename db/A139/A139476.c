/*
 * A139476  Positions of squares in the EKG sequence (A064413).
 * https://oeis.org/A139476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this code generates the EKG sequence and records the * indices
 *            at which perfect squares occur. The function a(n) returns * the
 *            position of n^2 in that sequence. The implementation keeps * a
 *            cached state so that repeated calls are fast
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 143, u64 n <= 143,
 *            u128 n <= 143, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ---------- configuration ---------- */
#define INITIAL_SQ_LIMIT 10000ULL           /* initial number of square positions stored */

/* ---------- helper bitset ----------
 * The used numbers bit‑set so we can test presence in O(1). */
static inline bool bit_test(const uint8_t *bits, uint32_t v)
{
    return (bits[v >> 3] >> (v & 7)) & 1U;
}
static inline void bit_set(uint8_t *bits, uint32_t v)
{
    bits[v >> 3] |= 1U << (v & 7);
}

/* ---------- state ----------
 * We maintain all tables needed for generation and the cached
 * positions of squares.  Only static helpers are required. */
typedef struct {
    uint32_t      limit;          /* maximum value considered in tables   */
    uint8_t      *used;           /* bitset of used numbers               */
    uint16_t     *spf;            /* smallest prime factor array          */
    uint32_t     *next;           /* next unused multiple of each prime   */
    seq_t        *pos;            /* pos[r] = position of r^2             */
    size_t        pos_size;       /* allocated size of pos array          */
    uint32_t      prev;           /* previous term in the EKG sequence    */
    uint32_t      idx;            /* current length of the sequence       */
    idx_t         max_sq_found;   /* largest square (as integer) found    */
} ekg_state_t;

static ekg_state_t *ekg = NULL;

/* ---------- build smallest‑prime‑factor sieve ----------
 * For each n up to limit we store the smallest prime factor. */
static void build_spf(ekg_state_t *e)
{
    for (uint32_t i = 2; i <= e->limit; ++i) {
        if (e->spf[i] == 0) {                  /* i is prime */
            e->spf[i] = (uint16_t)i;
            if ((uint64_t)i * i <= e->limit) {
                for (uint32_t j = i * i; j <= e->limit; j += i) {
                    if (e->spf[j] == 0)
                        e->spf[j] = (uint16_t)i;
                }
            }
        }
    }
}

/* ---------- initialise global state ----------
 * Called lazily on first use. */
static void init_state(void)
{
    ekg = (ekg_state_t*)malloc(sizeof(ekg_state_t));
    ekg->limit = 100000000U;                     /* 1e8 is comfortably above the values needed for n≈1e4 squares */
    ekg->used = (uint8_t*)calloc((ekg->limit >> 3) + 1, 1);
    ekg->spf  = (uint16_t*)calloc(ekg->limit + 1, sizeof(uint16_t));
    ekg->next = (uint32_t*)calloc(ekg->limit + 1, sizeof(uint32_t));
    ekg->pos_size = INITIAL_SQ_LIMIT + 1;
    ekg->pos = (seq_t*)calloc(ekg->pos_size, sizeof(seq_t));
    if (!ekg->used || !ekg->spf || !ekg->next || !ekg->pos)
        exit(1);                    /* memory allocation failed */

    build_spf(ekg);

    /* seed the EKG sequence */
    bit_set(ekg->used, 1);
    bit_set(ekg->used, 2);
    ekg->prev = 2;
    ekg->idx  = 2;
    ekg->max_sq_found = 1;          /* 1^2 appears at position 1 */
    ekg->pos[1] = 1;
}

/* ---------- extend the pos array when required ----------
 * The array is resized in powers of two to keep realloc overhead low. */
static void ensure_pos_capacity(idx_t need)
{
    if ((idx_t)ekg->pos_size > need)
        return;
    size_t new_size = ekg->pos_size * 2;
    if (new_size < (size_t)need + 1)
        new_size = (size_t)need + 1;
    seq_t *p = (seq_t*)realloc(ekg->pos, new_size * sizeof(seq_t));
    if (!p)
        exit(1);
    /* zero the newly added entries */
    memset(p + ekg->pos_size, 0, (new_size - ekg->pos_size) * sizeof(seq_t));
    ekg->pos = p;
    ekg->pos_size = new_size;
}

/* ---------- generate the next term of the EKG sequence ----------
 * The implementation follows the classic algorithm using prime
 * factors of the previous term and a table of next unused
 * multiples for each prime. */
static void generate_next(void)
{
    uint32_t x = ekg->prev;
    uint32_t rem = x;
    uint32_t primes_buf[16];
    int primes_cnt = 0;

    /* factorise x into distinct primes (primes_buf holds them) */
    while (rem > 1) {
        uint16_t sp = ekg->spf[rem];
        uint32_t p  = sp ? (uint32_t)sp : rem;
        if (primes_cnt == 0 || primes_buf[primes_cnt - 1] != p)
            primes_buf[primes_cnt++] = p;
        do {
            rem /= p;
        } while (rem % p == 0);
    }

    /* for every prime factor collect the smallest unused multiple */
    uint32_t cand_buf[16];
    for (int i = 0; i < primes_cnt; ++i) {
        uint32_t p   = primes_buf[i];
        uint32_t cand = ekg->next[p] ? ekg->next[p] : p;
        while (bit_test(ekg->used, cand))
            cand += p;
        ekg->next[p] = cand;
        cand_buf[i]  = cand;
    }

    /* choose the minimal candidate */
    uint32_t next_term = cand_buf[0];
    for (int i = 1; i < primes_cnt; ++i)
        if (cand_buf[i] < next_term)
            next_term = cand_buf[i];

    /* update state */
    ekg->prev = next_term;
    ekg->idx++;
    bit_set(ekg->used, next_term);

    /* check if it is a perfect square */
    seq_t r = floor_sqrt((seq_t)next_term);
    if ((seq_t)r * (seq_t)r == (seq_t)next_term) {
        idx_t r_idx = (idx_t)r;                /* r is guaranteed <= UINT32_MAX */
        if (r_idx >= 1) {
            ensure_pos_capacity(r_idx);
            if (ekg->pos[(size_t)r_idx] == 0) {
                ekg->pos[(size_t)r_idx] = ekg->idx;
                if (r_idx > ekg->max_sq_found)
                    ekg->max_sq_found = r_idx;
            }
        }
    }
}

/* ---------- public function ----------
 * Returns the position of n^2 in the EKG sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but safe return */

    if (!ekg)
        init_state();

    ensure_pos_capacity(n);
    while (ekg->max_sq_found < n)
        generate_next();

    return ekg->pos[(size_t)n];
}
