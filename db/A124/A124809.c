/*
 * A124809  Numbers of the form (square + 1) that are not squarefree.
 * https://oeis.org/A124809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049532(n)^2 + 1 (numbers of the form k²+1 that are not
 *            squarefree)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- utilities (Tonelli–Shanks for uint32_t) ---------- */
static uint32_t mod_pow32(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint32_t)result;
}

static uint32_t mod_inv32(uint32_t a, uint32_t mod)
{
    /* mod is prime, a != 0 */
    return mod_pow32(a, mod - 2, mod);
}

/* Tonelli–Shanks for a quadratic residue modulo a prime p (p odd). */
static uint32_t mod_sqrt32(uint32_t a, uint32_t p)
{
    if (a == 0) return 0;
    if (p == 2) return a;
    if (mod_pow32(a, (p - 1) / 2, p) != 1) return 0; /* not a residue */

    uint32_t q = p - 1;
    uint32_t s = 0;
    while ((q & 1) == 0) {
        q >>= 1;
        s++;
    }

    uint32_t z = 2;
    while (mod_pow32(z, (p - 1) / 2, p) != p - 1) ++z;

    uint64_t c = mod_pow32(z, q, p);
    uint64_t r = mod_pow32(a, (q + 1) / 2, p);
    uint64_t t = mod_pow32(a, q, p);
    uint32_t m = s;

    while (t != 1) {
        uint64_t tt = t;
        uint32_t i = 0;
        while (tt != 1) {
            tt = (tt * tt) % p;
            i++;
        }
        uint64_t b = mod_pow32((uint32_t)c, 1U << (m - i - 1), p);
        r = (r * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    return (uint32_t)r;
}

/* ---------- sieve for primes up to a given limit ---------- */
#define MAX_PRIME_LIMIT 1000000
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    uint8_t *is_comp = (uint8_t *)calloc(MAX_PRIME_LIMIT + 1, 1);
    if (!is_comp) exit(EXIT_FAILURE);
    size_t count = 0;
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if ((uint64_t)i * i <= MAX_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
            ++count;
        }
    }
    prime_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!is_comp[i]) prime_list[idx++] = i;
    prime_count = count;
    free(is_comp);
}

/* ---------- min‑heap for (k, step) pairs ---------- */
typedef struct {
    seq_t k;
    seq_t step;
} Node;

static Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

static void heap_init(void)
{
    heap_cap = 16;
    heap = (Node *)malloc(heap_cap * sizeof(Node));
    if (!heap) exit(EXIT_FAILURE);
    heap_size = 0;
}

static void heap_push(seq_t k, seq_t step)
{
    if (heap_size == heap_cap) {
        heap_cap *= 2;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
        if (!heap) exit(EXIT_FAILURE);
    }
    size_t i = heap_size++;
    heap[i].k = k;
    heap[i].step = step;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].k <= heap[i].k) break;
        Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static void heap_pop(Node *out)
{
    if (heap_size == 0) return;
    *out = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && heap[l].k < heap[smallest].k) smallest = l;
        if (r < heap_size && heap[r].k < heap[smallest].k) smallest = r;
        if (smallest == i) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* ---------- cache for computed a(n) values ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }
    cache[cache_len++] = val;
}

/* ---------- sequence initialization ---------- */
static bool seq_initialized = false;

static void init_sequence(void)
{
    if (seq_initialized) return;
    seq_initialized = true;

    init_primes();
    heap_init();

    /* For every prime p ≡ 1 (mod 4), compute the two residue classes
       of k modulo p² for k² ≡ –1 (mod p²). Push each class's initial element. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p % 4 != 1) continue;

        uint32_t r0 = mod_sqrt32(p - 1, p); /* sqrt of –1 mod p */
        if (r0 == 0) continue; /* safety, should not happen */

        /* Lift to modulus p² via Hensel's lemma */
        uint64_t x = (uint64_t)r0 * r0 + 1ULL;      /* divisible by p */
        uint64_t X = x / p;                        /* integer */
        uint32_t inv = mod_inv32((2 * r0) % p, p);  /* inverse of 2*r0 mod p */
        uint32_t k = (uint32_t)(((p - (X % p)) % p) * (uint64_t)inv % p);
        uint64_t r = (uint64_t)r0 + (uint64_t)k * p;  /* residue modulo p² */
        uint64_t modp2 = (uint64_t)p * (uint64_t)p;

        seq_t r1 = (seq_t)r;
        seq_t r2 = (seq_t)(modp2 - r);   /* the second residue */
        seq_t step = (seq_t)p * (seq_t)p;

        heap_push(r1, step);
        if (r2 != r1)
            heap_push(r2, step);
    }
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for n < 1 */

    init_sequence();

    static seq_t last_y = 0;
    while (cache_len < (size_t)n) {
        Node cur;
        heap_pop(&cur);
        seq_t k = cur.k;
        seq_t y = k * k + 1;

        if (cache_len == 0 || y != last_y) {
            cache_push(y);
            last_y = y;
        }

        cur.k += cur.step; /* advance to the next member of the arithmetic progression */
        heap_push(cur.k, cur.step);
    }
    return cache[n - 1];
}
