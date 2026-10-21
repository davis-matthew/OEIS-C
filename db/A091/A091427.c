/*
 * A091427  Write out the primes and then regroup in 3 digit blocks (omitting
 *          leading zeros).
 * https://oeis.org/A091427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ***********************
   OEIS A091427
   Write the primes in decimal, concatenate them, then split the
   resulting string into consecutive 3‑digit blocks (discard any
   leading zeros of a block).  The nth block (n ≥ 1) is a(n).

   Implementation details:
   * we generate primes incrementally, storing them in an array.
   * each new prime contributes its decimal digits to a global buffer.
   * every time the buffer length reaches a multiple of three we
   * form the corresponding block (an integer 0‑999) and store it
   * in another array.  Blocks are created in order, so a(n) is
   * simply blocks[n‑1].
   * The function is fully static, with memoisation between calls,
   * so any sequence of calls (not necessarily monotone) works.

   Complexity: O(n log n) prime generation for n ≤ 10 000; –
   far below the time limit.  All intermediates are 64‑bit or
   smaller, so no overflow occurs.
   **************************** */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_buf.h"

/* ----------  State  ---------- */
typedef struct {
    /* primes */
    uint64_t *primes;      /* array of primes found so far      */
    size_t    primes_cnt;  /* how many                         */
    size_t    primes_cap;  /* allocated capacity                */

    /* decimal digits of concatenated primes (0‑9 as numeric bytes) */
    uint8_t *digits;       /* digits[0] .. digits[digits_cnt-1] */
    size_t   digits_cnt;
    size_t   digits_cap;

    /* 3‑digit blocks */
    seq_t *blocks;         /* blocks[0] .. blocks[blocks_cnt‑1]  */
    size_t blocks_cnt;
    size_t blocks_cap;

    /* next candidate to test for primality */
    uint64_t next_candidate;
} state_t;

static state_t state = {0};

/* ----------  Append a decimal digit (0‑9)  ---------- */
static inline void append_digit(uint8_t d)
{
    grow_buf((void**)&state.digits, &state.digits_cnt, &state.digits_cap,
             sizeof(uint8_t), state.digits_cnt + 1);
    state.digits[state.digits_cnt++] = d;
}

/* ----------  Add a new prime and update state  ---------- */
static void add_prime(uint64_t p)
{
    /* convert the prime to decimal digits (little‑endian) */
    uint8_t buf[32];
    size_t blen = 0;
    uint64_t tmp = p;
    do {
        buf[blen++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* append digits in correct order */
    for (size_t i = 0; i < blen; ++i) append_digit(buf[blen - i - 1]);

    /* form any new blocks that completed */
    size_t want_blocks = state.digits_cnt / 3;
    while (state.blocks_cnt < want_blocks) {
        size_t idx = state.blocks_cnt * 3;
        seq_t val = (seq_t)(state.digits[idx] * 100 + state.digits[idx+1] * 10
                          + state.digits[idx+2]);
        grow_buf((void**)&state.blocks, &state.blocks_cnt, &state.blocks_cap,
                 sizeof(seq_t), state.blocks_cnt + 1);
        state.blocks[state.blocks_cnt++] = val;
    }

    /* remember the prime for future primality tests */
    grow_buf((void**)&state.primes, &state.primes_cnt, &state.primes_cap,
             sizeof(uint64_t), state.primes_cnt + 1);
    state.primes[state.primes_cnt++] = p;
}

/* ----------  Generate primes until we have at least target blocks  ---------- */
static void ensure_blocks(idx_t target)
{
    while (state.blocks_cnt < target) {
        uint64_t p = state.next_candidate;
        bool ok = true;
        for (size_t i = 0; i < state.primes_cnt; ++i) {
            uint64_t q = state.primes[i];
            if (q * q > p) break;
            if (p % q == 0) { ok = false; break; }
        }
        if (ok) add_prime(p);
        ++state.next_candidate;
    }
}

/* ----------  The required function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0 */
    if (state.next_candidate == 0) state.next_candidate = 2; /* initialise on first call */
    ensure_blocks((idx_t)n);
    return state.blocks[n-1];
}
