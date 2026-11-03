/*
 * A107627  Numbers n such that every digit of n and sqrt(n) contains a loop
 *          (only digits 0,4,6,8,9 in n and sqrt(n)).
 * https://oeis.org/A107627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, dumb
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that every digit of n and √n contains a loop
   (digits 0,4,6,8,9).  The terms are the squares of numbers composed only of
   those digits, and the squares themselves must also have only those digits.
   Generation is done breadth‑first on the roots, keeping the list of found
   terms in ascending order.                                                     */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ---------- Helper: test if every decimal digit is a loop digit ---------- */
static bool has_loop_digits_uint128(unsigned __int128 v)
{
    if (v == 0) return true;          /* 0 is a valid loop digit */
    while (v != 0) {
        unsigned d = (unsigned)(v % 10);
        if (d != 0 && d != 4 && d != 6 && d != 8 && d != 9)
            return false;
        v /= 10;
    }
    return true;
}

/* Compute root^2, safely handling possible overflows in seq_t.            */
static seq_t root_square(seq_t root)
{
    unsigned __int128 sq = (unsigned __int128)(seq_t)root * (unsigned __int128)(seq_t)root;
    seq_t max_val = (seq_t)-1;                    /* maximum value representable by seq_t */
    if (sq > (unsigned __int128)max_val) return 0; /* overflow -> treat as 0 (invalid) */
    return (seq_t)sq;
}

/* ---------- Dynamic storage for terms and the root queue ----------------- */
static seq_t *term_arr = NULL;   /* array of found terms (squares)   */
static idx_t term_cap = 0, term_cnt = 0;

static seq_t *queue_arr = NULL;   /* queue of roots to process        */
static idx_t queue_cap = 0;
static idx_t queue_pos = 0;       /* next element to pop              */
static idx_t queue_end = 0;       /* one past last element            */

/* Ensure capacity for the term array, doubling as needed.                 */
static void ensure_term_cap(idx_t needed)
{
    if (needed <= term_cap) return;
    idx_t new_cap = term_cap ? term_cap << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = mem_resize(seq_t, term_arr, new_cap);
    term_arr = new_arr;
    term_cap = new_cap;
}

/* Ensure capacity for the queue array.                                     */
static void ensure_queue_cap(idx_t needed)
{
    if (needed <= queue_cap) return;
    idx_t new_cap = queue_cap ? queue_cap << 1 : 256;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = mem_resize(seq_t, queue_arr, new_cap);
    queue_arr = new_arr;
    queue_cap = new_cap;
}

/* Push a new root into the queue.                                          */
static void queue_push(seq_t v)
{
    ensure_queue_cap(queue_end + 1);
    queue_arr[queue_end++] = v;
}

/* Initialise the queue with the single‑digit loop numbers.                 */
static void init_queue(void)
{
    if (queue_arr) return;   /* already initialised */
    queue_arr = NULL;
    queue_cap = 0;
    queue_pos = queue_end = 0;
    queue_push(4);
    queue_push(6);
    queue_push(8);
    queue_push(9);
}

/* Generate terms until we have at least n of them.                         */
static void generate_until(idx_t n)
{
    init_queue();
    while (term_cnt < n) {
        if (queue_pos >= queue_end) break;   /* safety, should not happen */

        seq_t root = queue_arr[queue_pos++];
        seq_t sq   = root_square(root);

        if (sq && has_loop_digits_uint128((unsigned __int128)sq)) {
            ensure_term_cap(term_cnt + 1);
            term_arr[term_cnt++] = sq;
        }

        /* Generate children: append each loop digit to the root. */
        const unsigned char digits[5] = {0, 4, 6, 8, 9};
        for (int i = 0; i < 5; ++i) {
            unsigned d = digits[i];
            unsigned __int128 child128 = (unsigned __int128)(seq_t)root * 10 + d;
            seq_t max_val = (seq_t)-1;
            if (child128 > (unsigned __int128)max_val) continue; /* overflow for seq_t */
            queue_push((seq_t)child128);
        }
    }
}

/* ---------- External function required by the test harness. ------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, guard against underflow */
    if (term_cnt < n) generate_until(n);
    return term_arr[n - 1];
}
