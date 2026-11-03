/*
 * A024777  Every prefix and suffix prime in base 9 (written in base 9).
 * https://oeis.org/A024777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A024777: every base‑9 number whose every prefix and suffix is prime
   (primality of a base‑9 number is tested on its decimal value).
   The function memoizes results and answers queries for any n (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Helper structures and static storage                                    */
/* --------------------------------------------------------------------- */
struct PNode {
    seq_t dec;        /* decimal value of the number                     */
    seq_t rep;        /* base‑9 representation as a decimal integer      */
    unsigned len;     /* number of base‑9 digits                         */
};

struct ResNode {
    seq_t dec;
    seq_t rep;
};

static bool initialized = false;
static seq_t *pow9 = NULL;          /* 9^k values for k >= 0                     */
static size_t pow9_len = 0, pow9_cap = 0;

static struct PNode *queue = NULL;  /* nodes to expand                          */
static size_t queue_len = 0, queue_cap = 0, queue_head = 0;

static struct ResNode *res = NULL;  /* sorted results                           */
static size_t res_len = 0, res_cap = 0;

static idx_t max_requested_n = 0;

/* --------------------------------------------------------------------- */
/* Dynamic array helpers                                                   */
/* --------------------------------------------------------------------- */
static void ensure_pow9(size_t n) {
    if (pow9_len > n) return;
    while (pow9_len <= n) {
        size_t new_cap = pow9_cap ? pow9_cap * 2 : 8;
        if (new_cap < pow9_len + 1) new_cap = pow9_len + 1;
        if (new_cap != pow9_cap) {
            pow9_cap = new_cap;
            pow9 = mem_resize(seq_t, pow9, pow9_cap);
        }
        if (pow9_len == 0) {
            pow9[0] = (seq_t)1;
        } else {
            pow9[pow9_len] = pow9[pow9_len - 1] * (seq_t)9;
        }
        pow9_len++;
    }
}

static void ensure_queue_cap(void) {
    if (queue_cap == 0) {
        queue_cap = 8;
        queue = mem_new(struct PNode, queue_cap);
    } else if (queue_len == queue_cap) {
        queue_cap *= 2;
        queue = mem_resize(struct PNode, queue, queue_cap);
    }
}

static void push_queue(struct PNode node) {
    ensure_queue_cap();
    queue[queue_len++] = node;
}

static void ensure_res_cap(void) {
    if (res_cap == 0) {
        res_cap = 16;
        res = mem_new(struct ResNode, res_cap);
    } else if (res_len == res_cap) {
        res_cap *= 2;
        res = mem_resize(struct ResNode, res, res_cap);
    }
}

static void insert_result(seq_t dec, seq_t rep) {
    /* Insert into the sorted res array maintaining order by decimal value */
    size_t pos = 0;
    while (pos < res_len && res[pos].dec < dec) pos++;
    ensure_res_cap();
    if (pos < res_len) {
        mem_move(&res[pos + 1], &res[pos], (res_len - pos) * sizeof(struct ResNode));
    }
    res[pos].dec = dec;
    res[pos].rep = rep;
    res_len++;
}

/* --------------------------------------------------------------------- */
/* Core generation loop                                                   */
/* --------------------------------------------------------------------- */
static void generate_up_to(size_t target) {
    while (res_len < target) {
        if (queue_head >= queue_len) break;   /* no more nodes to expand */

        struct PNode cur = queue[queue_head++];

        /* Test suffix property for the current node */
        bool ok = true;
        if (cur.len > 1) {
            ensure_pow9(cur.len - 1);
            for (unsigned k = 1; k < cur.len; ++k) {
                seq_t suffix = cur.dec % pow9[k];
                if (!is_prime_big(suffix)) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) insert_result(cur.dec, cur.rep);

        /* Generate children (extend by a new base‑9 digit) */
        for (int d = 0; d <= 8; ++d) {
            seq_t new_dec = cur.dec * (seq_t)9 + (seq_t)d;
            if (!is_prime_big(new_dec)) continue;
            seq_t new_rep = cur.rep * (seq_t)10 + (seq_t)d;
            struct PNode child = { new_dec, new_rep, cur.len + 1 };
            push_queue(child);
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public interface                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* idx_t is unsigned, but guard against 0 */

    /* One‑time global initialization */
    if (!initialized) {
        /* Allocate pow9 array with at least one element */
        pow9_cap = 8;
        pow9 = mem_new(seq_t, pow9_cap);
        pow9[0] = (seq_t)1;
        pow9_len = 1;

        /* Seed the queue with single‑digit base‑9 primes (2,3,5,7) */
        const seq_t primes[] = {2, 3, 5, 7};
        for (size_t i = 0; i < 4; ++i) {
            struct PNode node = {primes[i], primes[i], 1};
            push_queue(node);
        }
        initialized = true;
    }

    size_t target = (size_t)n;
    if (target > max_requested_n) {
        max_requested_n = target;
        generate_up_to(target);
    }
    return res[target - 1].rep;
}
