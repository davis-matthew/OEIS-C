/*
 * A024774  Every prefix and suffix prime in base 6 (written in base 6).
 * https://oeis.org/A024774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   cmp_seq_t, ensure_pow6, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate every base‑6 number such that each prefix and
   suffix (interpreted as base‑6) is prime. The function returns the
   base‑6 representation written as a decimal integer. */

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/is_prime_big.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/ensure_pow6.h"

/* Data structures */
typedef struct {
    seq_t val;             /* value interpreted as a decimal integer of the
                              number written in base‑6 (e.g. 17 for “25”). */
    unsigned char len;     /* number of base‑6 digits in the number. */
} Node;

/* ---------- heap utilities ---------- */
static inline int cmp_node(const Node *a, const Node *b)
{
    /* min‑heap on the decimal value of the base‑6 number */
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    return 0;
}

static void grow_heap(Node **arr, size_t *cap, size_t *size, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
    while (newcap < need) newcap <<= 1;
    Node *tmp = (Node *)malloc(sizeof(Node) * newcap);
    for (size_t i = 0; i < *size; ++i) tmp[i] = (*arr)[i];
    free(*arr);
    *arr   = tmp;
    *cap   = newcap;
    *size  = *size; /* unchanged */
}

static void heap_push(Node **arr, size_t *size, size_t *cap, const Node *value)
{
    grow_heap(arr, cap, size, *size + 1);
    size_t i = *size;
    (*arr)[i] = *value;
    ++(*size);
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (cmp_node(&(*arr)[i], &(*arr)[parent]) < 0) {
            Node tmp = (*arr)[i];
            (*arr)[i] = (*arr)[parent];
            (*arr)[parent] = tmp;
            i = parent;
        } else break;
    }
}

static Node heap_pop(Node **arr, size_t *size, size_t *cap)
{
    Node root = (*arr)[0];
    --(*size);
    if (*size > 0) {
        (*arr)[0] = (*arr)[*size];
        size_t i = 0;
        while (1) {
            size_t left = i * 2 + 1;
            if (left >= *size) break;
            size_t right = left + 1;
            size_t smallest = left;
            if (right < *size && cmp_node(&(*arr)[right], &(*arr)[left]) < 0)
                smallest = right;
            if (cmp_node(&(*arr)[smallest], &(*arr)[i]) < 0) {
                Node tmp = (*arr)[i];
                (*arr)[i] = (*arr)[smallest];
                (*arr)[smallest] = tmp;
                i = smallest;
            } else break;
        }
    }
    return root;
}

/* ---------- cache utilities ---------- */
static void grow_cache(seq_t **arr, size_t *cap, size_t *size, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * newcap);
    for (size_t i = 0; i < *size; ++i) tmp[i] = (*arr)[i];
    free(*arr);
    *arr   = tmp;
    *cap   = newcap;
    *size  = *size; /* unchanged */
}

static void cache_push(seq_t **arr, size_t *size, size_t *cap, const seq_t *value)
{
    grow_cache(arr, cap, size, *size + 1);
    (*arr)[*size] = *value;
    ++(*size);
}

/* ---------- helper: base‑6 digit to decimal integer ---------- */
static seq_t base6_to_decimal(seq_t val, unsigned len)
{
    uint8_t digits[64];
    seq_t tmp = val;
    for (unsigned i = 0; i < len; ++i) {
        digits[i] = (uint8_t)(tmp % ((seq_t)6));
        tmp /= ((seq_t)6);
    }
    seq_t rep = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        rep = rep * ((seq_t)10) + ((seq_t)digits[i]);
    }
    return rep;
}

/* ---------- suffix prime test ---------- */
static bool all_suffixes_prime(const seq_t *pow6, size_t pow_cap,
                               const seq_t val, int len)
{
    for (int k = 1; k <= len; ++k) {
        seq_t suffix = val % pow6[k];
        if (!is_prime_big(suffix)) return false;
    }
    return true;
}

/* ---------- main state and algorithm ---------- */
static bool state_inited = false;
static Node  *heap      = NULL;
static size_t heap_size = 0, heap_cap = 0;

static seq_t *cache     = NULL;
static size_t cache_size = 0, cache_cap = 0;

static seq_t *pow6     = NULL;
static size_t pow6_cap = 0;

/* generate numbers until we have at least `need` terms */
static void ensure_generated(idx_t need)
{
    if (!state_inited) {
        /* initial root primes: 2,3,5 */
        const unsigned char init_digits[] = {2, 3, 5};
        for (size_t i = 0; i < 3; ++i) {
            Node node = { .val = (seq_t)init_digits[i], .len = 1 };
            heap_push(&heap, &heap_size, &heap_cap, &node);
        }
        state_inited = true;
    }

    if (!pow6) {
        pow6     = (seq_t *)malloc(sizeof(seq_t));
        pow6[0]  = (seq_t)1;
        pow6_cap = 1;
    }

    while (cache_size < need) {
        if (heap_size == 0) break;               /* no more candidates */

        Node cur = heap_pop(&heap, &heap_size, &heap_cap);

        ensure_pow6(&pow6, &pow6_cap, (size_t)cur.len);

        if (all_suffixes_prime(pow6, pow6_cap, cur.val, cur.len)) {
            seq_t rep = base6_to_decimal(cur.val, cur.len);
            cache_push(&cache, &cache_size, &cache_cap, &rep);
        }

        for (int d = 0; d <= 5; ++d) {
            seq_t new_val = cur.val * ((seq_t)6) + (seq_t)d;
            if (is_prime_big(new_val)) {
                Node child = { .val = new_val, .len = cur.len + 1 };
                heap_push(&heap, &heap_size, &heap_cap, &child);
            }
        }
    }
}

/* ---------- requested function ---------- */
seq_t a(idx_t n)
{
    /* generate until we have at least n terms */
    ensure_generated(n);
    return cache[n - 1];
}
