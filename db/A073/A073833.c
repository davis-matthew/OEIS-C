/*
 * A073833  Numerators of b(n) where b(1) = 1, b(i) = b(i-1) + 1/b(i-1).
 * https://oeis.org/A073833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, changed
 * Formula:   a(n) = numerator of b(n), where b(1)=1 and b(i)=b(i-1)+1/b(i-1).
 *            Recurrence (let p(n),q(n) be the reduced numerator/denominator
 *            of b(n)): p(1)=q(1)=1; p(n)=p(n-1)^2+q(n-1)^2;
 *            q(n)=p(n-1)*q(n-1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation is memoised so that calls in arbitrary order
   recompute only what is necessary. */
#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset – value irrelevant */

#if defined(__cplusplus)
    /* C++ implementation using std::vector */
    static std::vector<seq_t> p_vec(2);  /* indices 0..1 ; 0 unused */
    static std::vector<seq_t> q_vec(2);
    static idx_t computed = 0;         /* highest index for which values are known */

    if (computed == 0) {
        /* first initialisation */
        p_vec[1] = (seq_t)1;
        q_vec[1] = (seq_t)1;
        computed = 1;
    }

    /* enlarge storage if necessary */
    if ((size_t)n >= p_vec.size()) {
        size_t new_size = p_vec.size() * 2;
        if (new_size <= (size_t)n) new_size = n + 1;
        p_vec.resize(new_size);
        q_vec.resize(new_size);
    }

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t p_prev = p_vec[i - 1];
        seq_t q_prev = q_vec[i - 1];
        seq_t p_new = p_prev * p_prev + q_prev * q_prev;
        seq_t q_new = p_prev * q_prev;
        p_vec[i] = p_new;
        q_vec[i] = q_new;
    }
    computed = n;
    return p_vec[n];

#else
    /* C implementation using malloc/realloc */
    static seq_t *p_arr = NULL;
    static seq_t *q_arr = NULL;
    static idx_t allocated = 0;        /* number of elements allocated (>=2) */
    static idx_t computed = 0;         /* highest index computed */

    if (p_arr == NULL) {
        allocated = 2;                 /* indices 0..1 */
        p_arr = (seq_t *)malloc(sizeof(seq_t) * allocated);
        q_arr = (seq_t *)malloc(sizeof(seq_t) * allocated);
        p_arr[1] = (seq_t)1;
        q_arr[1] = (seq_t)1;
        computed = 1;
    }

    /* grow to accommodate n */
    if ((size_t)n >= allocated) {
        size_t new_alloc = allocated * 2;
        if (new_alloc <= (size_t)n) new_alloc = n + 1;
        p_arr = (seq_t *)realloc(p_arr, sizeof(seq_t) * new_alloc);
        q_arr = (seq_t *)realloc(q_arr, sizeof(seq_t) * new_alloc);
        allocated = new_alloc;
    }

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t p_prev = p_arr[i - 1];
        seq_t q_prev = q_arr[i - 1];
        seq_t p_new = p_prev * p_prev + q_prev * q_prev;
        seq_t q_new = p_prev * q_prev;
        p_arr[i] = p_new;
        q_arr[i] = q_new;
    }
    computed = n;
    return p_arr[n];
#endif
}
