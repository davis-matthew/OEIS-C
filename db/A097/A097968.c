/*
 * A097968  Consider the succession of single digits of the positive even
 *          integers: 2 4 6 8 1 0 1 2 1 4 1 6 1 8 2 0 2 2 2 4 ... (A036211).
 *          This sequence is the lexicographically earliest sequence of
 *          distinct positive odd integers that produces the same succession
 *          of digits.
 * https://oeis.org/A097968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 67, u128 n <= 8261,
 *            big any n
 * Helpers:   seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest partition of the even‑digit stream
 * into distinct positive odd integers.  The stream is produced
 * by concatenating the decimal representations of the even numbers
 * 2,4,6,8,10,12….
 * For each term we choose the shortest prefix of the current
 * position that starts with a non‑zero digit, ends with an odd digit,
 * the next digit (if any) is not zero, and the integer has not
 * appeared before.  This greedy rule yields the sequence A097968. */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

#include "../utils/seq_from_str.h"
#include "../utils/seq_to_str.h"

/* ---------- even‑digit stream generator ---------- */

static seq_t even_cur;
static char even_buf[2000];
static size_t even_len;
static size_t even_idx;
static bool even_initialized = false;

static void init_even(void)
{
    even_cur = 2;
    seq_to_str(even_cur, even_buf, &even_len);
    even_idx = 0;
    even_initialized = true;
}

static char next_even_digit(void)
{
    if (!even_initialized)
        init_even();

    if (even_idx >= even_len) {
        even_cur += 2;
        seq_to_str(even_cur, even_buf, &even_len);
        even_idx = 0;
    }
    return even_buf[even_idx++];
}

/* ---------- stream buffer for digits ---------- */

static char *stream_buf = NULL;
static size_t stream_len  = 0;   /* number of digits stored */
static size_t stream_cap  = 0;
static size_t read_pos    = 0;   /* next digit to consume */

/* ensure that stream_buf contains at least indices 0 .. pos */
static void ensure_stream_pos(size_t pos)
{
    while (stream_len <= pos) {
        char d = next_even_digit();
        if (stream_len == stream_cap) {
            size_t new_cap = stream_cap ? stream_cap * 2 : 16;
            char *tmp = (char *)realloc(stream_buf, new_cap);
            stream_buf = tmp;
            stream_cap = new_cap;
        }
        stream_buf[stream_len++] = d;
    }
}

/* ---------- cache of produced sequence terms ---------- */

static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- temporary buffer for candidate string ---------- */

static char *cand_buf  = NULL;
static size_t cand_cap = 0;

/* Build the next term of A097968 and store it in cache. */
static void build_next_term(void)
{
    size_t candidate_len = 0;

    while (true) {
        /* fetch the next digit of the current candidate */
        ensure_stream_pos(read_pos + candidate_len);
        char d = stream_buf[read_pos + candidate_len];
        candidate_len++;

        /* check if we can accept this prefix */
        if (stream_buf[read_pos] != '0' && ((d - '0') & 1)) {
            /* ensure we have the following digit to test zero start */
            ensure_stream_pos(read_pos + candidate_len);
            char next_d = stream_buf[read_pos + candidate_len];
            if (next_d == '0')
                continue;   /* next term would start with 0 -> reject */

            /* create null‑terminated string for conversion */
            if (cand_cap < candidate_len + 1) {
                size_t new_cap = cand_cap ? cand_cap * 2 : 16;
                while (new_cap < candidate_len + 1)
                    new_cap *= 2;
                char *tmp = (char *)realloc(cand_buf, new_cap);
                cand_buf = tmp;
                cand_cap = new_cap;
            }
            memcpy(cand_buf, &stream_buf[read_pos], candidate_len);
            cand_buf[candidate_len] = '\0';

            seq_t candidate = seq_from_str(cand_buf);

            /* check distinctness */
            bool dup = false;
            for (size_t i = 0; i < cache_size; ++i) {
                if (cache[i] == candidate) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                /* accept the candidate */
                ensure_cache_cap(cache_size + 1);
                cache[cache_size++] = candidate;
                read_pos += candidate_len;
                return;
            }
        }
    }
}

/* =========================================================
 *    Public function required by the evaluator
 * =========================================================*/

/* Return the n-th term of sequence A097968 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but harmless */

    while (cache_size < (size_t)n)
        build_next_term();

    return cache[n - 1];
}
